/* Based on https://bewareofgeek.livejournal.com/2945.html */

#include <sys/socket.h>
#include <linux/netlink.h>
#include <linux/connector.h>
#include <linux/cn_proc.h>
#include <signal.h>
#include <errno.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pwd.h>
#include <sys/stat.h>


#define PROC_DIR "/proc/"
#define CMDLINE "/cmdline"
/* max 65535 + \0 */
#define CMDLINE_PATH_LEN (strlen(CMDLINE)+strlen(PROC_DIR)+5+1)
#define STAT_PATH_LEN (strlen(PROC_DIR)+5+1)

#define DEAD_PROCESS "[undefined]"

char *get_proc_owner(pid_t pid);
char *cache_search(uid_t uid);
void *cache_add(uid_t uid);
void clean_cmdline(char *cmd, unsigned int bytes);


typedef struct s_cache_entry {
    uid_t key ;
    char *data ;
} Cache_ent ;

unsigned int cache_size = 0 ;
Cache_ent **cache;


void *cache_add(uid_t uid){

    Cache_ent *new = malloc(sizeof(Cache_ent));
    struct passwd *pwd_infos ;

    /* Ajust cache size */
    cache_size++;
    cache = realloc(cache, cache_size * sizeof(Cache_ent*));

    /* Create new entry */
    new->key = uid ;
    pwd_infos = getpwuid(uid);
    new->data = malloc(strlen(pwd_infos->pw_name) + 1);
    strncpy(new->data, pwd_infos->pw_name, strlen(pwd_infos->pw_name));
    new->data[strlen(pwd_infos->pw_name)] = 0 ;
    
    /* add to cache */
    cache[cache_size-1] = new ;

    /* Don't free getpwuid */
}

char *cache_search(uid_t uid){

    int i ;
    for(i=0; i<cache_size; i++){
        if(cache[i]->key  == uid){
           /* Cache hit */
           return cache[i]->data ;
        }
    }
    /* Cache miss */
    return NULL ;
}


void clean_cmdline(char *cmd, unsigned int bytes){

    int i ;
    printf("%s, %d\n",cmd, bytes);
    for(i = 0 ; i < bytes - 1 ;i++){
        if(cmd[i] == 0)
            cmd[i] = ' ';
    }
}



/*
 * connect to netlink
 * returns netlink socket, or -1 on error
 */
static int nl_connect()
{
    int rc;
    int nl_sock;
    struct sockaddr_nl sa_nl;

    nl_sock = socket(PF_NETLINK, SOCK_DGRAM, NETLINK_CONNECTOR);
    if (nl_sock == -1) {
        perror("socket");
        return -1;
    }

    sa_nl.nl_family = AF_NETLINK;
    sa_nl.nl_groups = CN_IDX_PROC;
    sa_nl.nl_pid = getpid();

    rc = bind(nl_sock, (struct sockaddr *)&sa_nl, sizeof(sa_nl));
    if (rc == -1) {
        perror("bind");
        close(nl_sock);
        return -1;
    }

    return nl_sock;
}

/*
 * subscribe on proc events (process notifications)
 */
static int set_proc_ev_listen(int nl_sock, bool enable)
{
    int rc;
    struct __attribute__ ((aligned(NLMSG_ALIGNTO))) {
        struct nlmsghdr nl_hdr;
        struct __attribute__ ((__packed__)) {
            struct cn_msg cn_msg;
            enum proc_cn_mcast_op cn_mcast;
        };
    } nlcn_msg;

    memset(&nlcn_msg, 0, sizeof(nlcn_msg));
    nlcn_msg.nl_hdr.nlmsg_len = sizeof(nlcn_msg);
    nlcn_msg.nl_hdr.nlmsg_pid = getpid();
    nlcn_msg.nl_hdr.nlmsg_type = NLMSG_DONE;

    nlcn_msg.cn_msg.id.idx = CN_IDX_PROC;
    nlcn_msg.cn_msg.id.val = CN_VAL_PROC;
    nlcn_msg.cn_msg.len = sizeof(enum proc_cn_mcast_op);

    nlcn_msg.cn_mcast = enable ? PROC_CN_MCAST_LISTEN : PROC_CN_MCAST_IGNORE;

    rc = send(nl_sock, &nlcn_msg, sizeof(nlcn_msg), 0);
    if (rc == -1) {
        perror("netlink send");
        return -1;
    }

    return 0;
}

/* get owner
int len, pid, n, fd = open("/proc/12345/status", O_RDONLY | O_NOATIME);
char buf[4096], whitespace[50];

if (0 < (len = read(fd, buf, 4096)))
{
    n = sscanf(buf, "Uid:%s%d ", whitespace, &pid);
}

*/

char *get_proc_owner(pid_t pid){

    struct stat sb;
    char *fname = malloc(STAT_PATH_LEN);

    snprintf(fname, STAT_PATH_LEN, PROC_DIR "%u", pid);

    if (stat(fname, &sb) == -1) {
        puts("Process no longer exists");
        return DEAD_PROCESS;
    }

    char *getname ;
    getname = cache_search(sb.st_uid);
    
    if(!getname){
        cache_add(sb.st_uid);
        return cache_search(sb.st_uid);
    }
    
    return getname;

}

/*
 * handle a single process event
 */
static volatile bool need_exit = false;
static int handle_proc_ev(int nl_sock)
{
    int rc;
    struct __attribute__ ((aligned(NLMSG_ALIGNTO))) {
        struct nlmsghdr nl_hdr;
        struct __attribute__ ((__packed__)) {
            struct cn_msg cn_msg;
            struct proc_event proc_ev;
        };
    } nlcn_msg;
    char cmdline_path[CMDLINE_PATH_LEN], cmdbuf[1024];

    while (!need_exit) {

        pid_t targetpid ;
        unsigned int bytes ;
        rc = recv(nl_sock, &nlcn_msg, sizeof(nlcn_msg), 0);
        if (rc == 0) {
            /* shutdown? */
            return 0;
        } else if (rc == -1) {
            if (errno == EINTR) continue;
            perror("netlink recv");
            return -1;
        }
        memset(cmdline_path, 0, CMDLINE_PATH_LEN);
        memset(cmdbuf, 0, 1024);
        switch (nlcn_msg.proc_ev.what) {
            case PROC_EVENT_FORK:
                targetpid = nlcn_msg.proc_ev.event_data.fork.child_pid ;
                break;
            case PROC_EVENT_EXEC:
                targetpid = nlcn_msg.proc_ev.event_data.exec.process_pid ;
                break;
        }
        if(targetpid){

            snprintf(cmdline_path, CMDLINE_PATH_LEN , PROC_DIR "%u" CMDLINE, targetpid);
            printf( "[%d] : ", targetpid );

            FILE *cmd = fopen(cmdline_path,"r");

            if(cmd){
                bytes = fread(&cmdbuf, 1, 1024, cmd);
                if(bytes > 1){
                    clean_cmdline(cmdbuf, bytes); 
                }
                printf("user: %s\t%s\n",get_proc_owner(targetpid), cmdbuf);
            }
            else {
                printf("- \n");
            }
        }
    }

    return 0;
}

static void on_sigint(int unused)
{
    need_exit = true;
}

int main(int argc, const char *argv[])
{
    int nl_sock;
    int rc = EXIT_SUCCESS;


    signal(SIGINT, &on_sigint);
    siginterrupt(SIGINT, true);

    nl_sock = nl_connect();
    if (nl_sock == -1)
        exit(EXIT_FAILURE);

    rc = set_proc_ev_listen(nl_sock, true);
    if (rc == -1) {
        rc = EXIT_FAILURE;
        goto out;
    }

    rc = handle_proc_ev(nl_sock);
    if (rc == -1) {
        rc = EXIT_FAILURE;
        goto out;
    }

	set_proc_ev_listen(nl_sock, false);

out:
    free(cache);
    close(nl_sock);
    exit(rc);
}
