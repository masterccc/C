#include <stdio.h>
#include <string.h>

typedef unsigned char u_byte;

void swap(u_byte *tab, int i1, int i2)
{
	u_byte tmp ;
	tmp = tab[i1] ;
	tab[i1] = tab[i2] ;
	tab[i2] = tmp ;
}

void key_schedule(u_byte tab_permut[255], u_byte key[], size_t ksize){
	int i, j = 0;
	for(i=0;i<=255;i++){
		j = ( j + tab_permut[i] + key[ i % ksize]) % 256 ;
		swap(tab_permut, i, j);
	}
}

void print_tab(u_byte t[255]){
	int j;
	puts("==========================");
	for(j=0;j<=255;j++){
		printf("%d ", t[j]);
	}
	puts("\n==========================");
}


void gen_cipher(u_byte tab_permut[255], u_byte *msg, size_t msize, int op){
	
	int i, j, count ;
	i = j = 0;
	u_byte cipher_b ;

	for(count = 0 ; count < msize ; count++){
		i = (i +1)%256 ;
		j = (j + tab_permut[i]) % 256 ;
		swap(tab_permut, i, j);
		cipher_b = tab_permut[ (tab_permut[i] + tab_permut[j]) % 256 ] ;
		if(!op)
			printf("%02x", cipher_b ^ msg[count]);	
		else
			printf("%c", cipher_b ^ msg[count]);
	}
}

void prepare(u_byte tab[255], u_byte key[], size_t ksize){
	
	int j ;

	for(j = 0; j <= 255; j++){
		tab[j] = j ;
	}

	key_schedule(tab, key, ksize);
}


int main(void){

	u_byte key[] = "MaSuperClee!";
	size_t ksize = strlen(key) ;

	u_byte msg[] = "Voici le message a chiffrer";
	size_t msize = strlen(msg);

	u_byte cipher[] = { 
			0x8c,0x35,0xdd,0x85,0xe2,0xd7,0x59,0xd4,
			0xfd,0x97,0x5d,0xbe,0xb1,0x31,0x53,0x3a,
			0x47,0xa0,0xa7,0x2b,0x3d,0x84,0x7f,0x21,
			0xbf,0x78,0x7a
	};
	size_t csize = 27;

	u_byte tab_permut[256];
	u_byte i;
	int j;

	prepare(tab_permut, key, ksize);
	puts("Chiffrement :");
	gen_cipher(tab_permut, msg, msize, 0);

	
	prepare(tab_permut, key, ksize);
	puts("\nDechiffrement :");
	gen_cipher(tab_permut, cipher, csize, 1);

	fflush(stdout);

	return 0;
}