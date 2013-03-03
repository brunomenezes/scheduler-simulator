/*
	execute o programa passando os 3 argumentos necessários;
	./programa <nome_arquivo_leitura> <tamanho_arquivo> <quantidade_io_cpu_burst
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Process Process;
typedef struct list List;
typedef struct listObj Obj;
typedef struct scheduler Scheduler;
typedef struct cpuBurst CpuBurst;
typedef struct ioBurst IOBurst;
typedef struct schedulerInfo SchedulerInfo;

struct cpuBurst{
	int burstTime;
	CpuBurst *next;
};

struct ioBurst
{
	int burstTime;
	IOBurst *next;
};

struct Process 
{
		char name[10];
		int priority, admission, id;
		CpuBurst *cpu_burst_init;
		IOBurst *io_burst_init;
};

struct listObj {
	Process *process;
	Obj *next;
	Obj *prev;
};

struct list {
	Obj *first;
};

struct schedulerInfo{
	int io_done, cpu_done;
	char status;

};

struct scheduler
{
	SchedulerInfo info;
	int processId;
};


void setProcesses(FILE *file, Process*);
void setProcess(Process *, char *ch, int attrIndex, int);
char *setInput(char *buffer,int in_tam);
void schedulerStart(Process *);


int main(int argc, char **argv){	
	int i,j,SIZE = atoi(argv[2]);
	Process *processes;
	processes = (Process *) malloc(sizeof(Process) * SIZE);
	FILE *file;
	file = fopen(argv[1], "r");

	setProcesses(file, processes);
	//schedulerStart(processes);


	/*
		implementar codigo de escalonamento com preempção e aging
	*/

	// codigo abaixo só para mostrar que está lendo direito o arquivo de teste
	for (i = 0; i < SIZE; ++i)
	{
		printf("process Id: %d \n",processes[i].id);
		printf("process name:%s\n",processes[i].name );
		printf("priority:%d\n",processes[i].priority );
		printf("process admission : %d\n",processes[i].admission );
		IOBurst *io;
		CpuBurst *cpu;
		cpu = processes[i].cpu_burst_init;
		io = processes[i].io_burst_init;
		while(cpu || io){
			if(cpu){
				printf("cpu: %d \n", cpu->burstTime);
				cpu = cpu->next;	
			}
			if(io){
				printf("io:%d\n",io->burstTime);
				io = io->next;
			}
				
		}
		printf("#################################################\n");
	}
	free(processes);

	return 0;
}


void setProcesses(FILE *file, Process *processList){
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
			setProcess(&processList[processIndex], input, attrIndex, processIndex);
			free(input);
			attrIndex++;
			index = 0;
		}
	
		ch = fgetc(file);
	}
	fclose(file);
}  

void setProcess(Process* p, char *ch, int attrIndex, int processIndex) {

	switch(attrIndex){
		case 0:
			p->id = (processIndex+1);
			p->admission = atoi(ch);
			p->cpu_burst_init = NULL;
			p->io_burst_init = NULL;
			break;
		case 1: 
			strcpy(p->name, ch);
			break;
		case 2:
			p->priority = atoi(ch);
			break;
		default:
			if(attrIndex % 2 == 1){
				CpuBurst *obj, *aux;
				obj = (CpuBurst *) malloc(sizeof(CpuBurst));
				obj->burstTime = atoi(ch);
				obj->next = NULL;
				if(p->cpu_burst_init == NULL){
					p->cpu_burst_init = obj;
				} else {
					aux = p->cpu_burst_init;
					while(aux->next != NULL)
						aux = aux->next;
					aux->next = obj;
				}
				//p->cpuBurst[p->cpuIndex] = 
				//p->cpuIndex += 1;

			} else {
				IOBurst *obj, *aux;
				obj = (IOBurst *) malloc(sizeof(IOBurst));
				obj->burstTime = atoi(ch);
				obj->next = NULL;
				if(p->io_burst_init == NULL){
					p->io_burst_init = obj;
				} else {
					aux = p->io_burst_init;
					while(aux->next != NULL)
						aux = aux->next;
					aux->next = obj;
				}
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

void schedulerStart(Process *processes){
	List *newQueue;
	List *readyQueue;
	List *waitQueue;
	Scheduler scheduler;


}

