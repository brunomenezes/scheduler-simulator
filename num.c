#include <stdio.h> 
#include <string.h>

int main (){

	FILE *entrada;
	int letra;
	entrada = fopen("saida", "r"); //abre para leitura! saida = fopen("arquivo2.txt", "w"); //abre para escrita 
	letra = fgetc(entrada);
	while(letra != EOF){ //le arquivo1
		printf("%c",letra);
		letra = fgetc(entrada);

	}
	return 0;
}