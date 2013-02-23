#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void show(char *ch);
void setPessoa(void *p , char *info);


typedef struct Pessoa {
	char name[10];
};

int main(){
	char name[] = "bruno";
	show(name);
	return 0;
}

void show(char *ch){
	Pessoa pessoas[2];
	setPessoa(&pessoas[0], ch);
	printf("nome: %s", pessoas[0].name);
}

void setPessoa(void *p, char *info){
	struct Pessoa *pessoa = (struct Pessoa *) p;
	strcpy((*pessoa).name, info);
}
