#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void show(char *ch);
void setPessoa(void *p , char *info);


typedef struct Pessoa {
	char name[10];
	int number;
}Pessoa;

int main(){
	char ch, input[15], *msg;
	int i=0;
	FILE *file = 
	file = fopen("teste.txt", "r");
	ch = fgetc(file);
	int spaces;
	while(ch != EOF) {
		if(ch != '\t' && ch != '\n'){
			printf("%c\n",ch );
			spaces = 0;
			input[i] = ch;
			i++;		
		} 
		if(ch == '\n' && spaces == 0){
			msg = (char *) malloc(sizeof(char) * (i+1));
			strcpy(msg,input);
			puts(msg);
			free(msg);
			i = 0;
			spaces++;
		}
		ch = fgetc(file);
	}
	return 0;
}

void show(char *ch){
	Pessoa *p;
	int i, pSize=10;
	printf("p adress : %d\n",&p);
	if(p == NULL){
		printf("p is point to null\n");
	} else {
		printf("p is point to %d\n",p );
	}
	p = (Pessoa *) calloc(pSize, sizeof(Pessoa));
	for (i = 0; i < pSize; ++i)
	{
		p[i].number = i*i;
	}

	free(p);

	// setPessoa(&pessoas[0], ch);
	// printf("nome: %s\n", pessoas[0].name);
}

void setPessoa(void *p, char *info){
	struct Pessoa *pessoa = (struct Pessoa *) p;
	strcpy((*pessoa).name, info);
}
