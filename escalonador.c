/*
	execute o programa passando os 3 argumentos necessários;
	./programa <nome_arquivo_leitura> <tamanho_arquivo> <quantidade_io_cpu_burst
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void setProcesses(FILE *file, void *p);
void setProcess(void *p, char *ch, int attrIndex);
char *setInput(char *buffer,int in_tam);

typedef struct Process {
		char name[10];
		int priority, admission, id, *cpuBurst, *ioBurst, ioIndex, cpuIndex;
}Process;

int main(int argc, char **argv){	
	int BURST_TIMES = atoi(argv[3]);
	int i,j,size = atoi(argv[2]);
	Process *processes;
	processes = (Process *) malloc(sizeof(Process) * size);
	FILE *file;
	file = fopen(argv[1], "r");
	for(i = 0 ; i < size ; i++){
		processes[i].ioBurst = (int *) malloc(sizeof(int)*BURST_TIMES);
		processes[i].cpuBurst = (int *) malloc(sizeof(int)*BURST_TIMES);
		processes[i].cpuIndex = 0;
		processes[i].ioIndex = 0;
	}
	setProcesses(file, processes);

	/*
		implementar codigo de escalonamento com preempção e aging
	*/

	// codigo abaixo só para mostrar que está lendo direito o arquivo de teste
	for (i = 0; i < size; ++i)
	{
		printf("process name:%s\n",processes[i].name );
		printf("priority:%d\n",processes[i].priority );
		printf("process admission : %d\n",processes[i].admission );
		for (j = 0; j < BURST_TIMES; j++)
		{
			printf("CPU :%d\n",processes[i].cpuBurst[j] );
			printf("IO :%d\n",processes[i].ioBurst[j] );
		}
		printf("#################################################\n");
	}
	free(processes);

	return 0;
}


void setProcesses(FILE *file, void *proc){
	Process *p = (struct Process *) proc;
	int TAM = 15;
	char buffer[TAM], *input, ch;
	int index = 0, processIndex = 0,attrIndex = 0;
	ch = fgetc(file);
	while(ch != EOF){
		if(ch != '\t' && ch != '\n'){
			buffer[index] = ch;
			index++;
		} else if(ch == '\n'){
			processIndex++;
			attrIndex = 0;
			index = 0;
		} else {
			input = setInput(buffer, index);
			setProcess(&p[processIndex], input, attrIndex);
			free(input);
			attrIndex++;
			index = 0;
		}
	
		ch = fgetc(file);
	}
	fclose(file);
}  

void setProcess(void *proc, char *ch, int attrIndex) {
	struct Process *p = (struct Process *) proc;
	switch(attrIndex){
		case 0:
			p->admission = atoi(ch);
			break;
		case 1: 
			strcpy(p->name, ch);
			break;
		case 2:
			p->priority = atoi(ch);
			break;
		default:
			if(attrIndex % 2 == 1){
				p->cpuBurst[p->cpuIndex] = atoi(ch);
				p->cpuIndex += 1;

			} else {
				p->ioBurst[p->ioIndex] = atoi(ch);
				p->ioIndex += 1;
			}
			break;
	}
}

char *setInput(char *buffer,int in_tam)
{
	char *input = (char *) calloc(in_tam,sizeof(char));
	int i;
	for (i = 0; i < in_tam; ++i)
	{
		*(input+i) = buffer[i];
	}
	return input;

}

