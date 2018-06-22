#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void alloc_and_affect(char ***res, char *str, int *size);

char **split(char  *s){

	char **res = NULL, tmp_word[128] = {0}, *tmp;
	int size = 0, space = 0 ;
	tmp = tmp_word;
	while(*s){
		if(*s == '\t' || *s == ' '){
			if(!space){
				alloc_and_affect(&res, tmp_word, &size);
				tmp = tmp_word ;
				memset(tmp,0,128);
			}
			space = 1 ;
		}
		else {
			space &= 0;
			*tmp++ = *s;
		}
		s++;
	}

	if(strlen(tmp_word))
		alloc_and_affect(&res, tmp_word, &size);
	size++;
	res = realloc(res, size * sizeof(char*));
	res[size-1] = NULL ;

	return res;
}


void alloc_and_affect(char ***res, char *str, int *size){
	*size = *size + 1;
	*res = realloc(*res, (*size) * sizeof(char*));
	(*res)[(*size)-1] = malloc( strlen(str) * sizeof(char));
	strcpy((*res)[(*size)-1], str);
}

int main(void){

	char **tab, *chaine = "a bbbb cccc  dddd		eeee 	ffff   g 	 	";
	int i;

	tab = split(chaine);

	for(i=0;tab[i];i++){
		printf("-%s\n",tab[i]);
	}

	return 0 ;
}


/*
char **tab; tab = malloc(5 * sizeof(*tab));
char ******tab; tab = malloc(5 * sizeof(*tab));
char ******tab; ****tab = malloc(5 * sizeof(*****tab));
 */
