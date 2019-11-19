#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

#define JE 0x74
#define JNE 0x75
#define RET 0xC3

int function(char *try){

    char *pass = "abcd";

    if(strcmp(pass,try) == 0){
        puts("Bypass");
        return 0 ;
    }
    else {
        return 1 ;	
    }
}

int change_perm(void *addr) {

    int page_size = getpagesize();
    addr -= (unsigned long)addr % page_size;

    if(mprotect(addr, page_size, PROT_READ | PROT_WRITE | PROT_EXEC) == -1) {
        fprintf(stderr, "mprotect() failed\n");
        return -1;
    }

    return 0;
}

int main(void){

    int i = 0;
    unsigned char* a;

    a = &function+i;

    while( *a++ != RET ){

        printf("Dump at %p ", a);
        printf("%02hhx \n", *a);

        if (*a == JNE && *(a+1) == 0x13){
            printf("JNE found ... Patching .... \n");
            change_perm(function);
            *a = JE ;
        }

        if( *a == RET) puts("    ^-- ret found --^");
        i++;

    }

    printf("Function returned %d\n", function("xxx"));

    return 0 ;
}


/*

0000000000001189 <function>:
    1189:   55                      push   rbp
    118a:   48 89 e5                mov    rbp,rsp
    118d:   48 83 ec 20             sub    rsp,0x20
    1191:   48 89 7d e8             mov    QWORD PTR [rbp-0x18],rdi
    1195:   48 8d 05 68 0e 00 00    lea    rax,[rip+0xe68]        # 2004 <_IO_stdin_used+0x4>
    119c:   48 89 45 f8             mov    QWORD PTR [rbp-0x8],rax
    11a0:   48 8b 55 e8             mov    rdx,QWORD PTR [rbp-0x18]
    11a4:   48 8b 45 f8             mov    rax,QWORD PTR [rbp-0x8]
    11a8:   48 89 d6                mov    rsi,rdx
    11ab:   48 89 c7                mov    rdi,rax
    11ae:   e8 9d fe ff ff          call   1050 <strcmp@plt>
    11b3:   85 c0                   test   eax,eax
    11b5:   75 13                   jne    11ca <function+0x41>
    11b7:   48 8d 3d 4b 0e 00 00    lea    rdi,[rip+0xe4b]        # 2009 <_IO_stdin_used+0x9>
    11be:   e8 6d fe ff ff          call   1030 <puts@plt>
    11c3:   b8 00 00 00 00          mov    eax,0x0
    11c8:   eb 05                   jmp    11cf <function+0x46>
    11ca:   b8 01 00 00 00          mov    eax,0x1
    11cf:   c9                      leave  
    11d0:   c3                      ret
*/