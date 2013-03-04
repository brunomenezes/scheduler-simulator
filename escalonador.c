#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define ON  1;
#define OFF 0;

typedef struct Process Process;
typedef struct list List;
typedef struct obj Obj;
typedef struct processor Processor;
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
		CpuBurst *cpuBursts;
		IOBurst *ioBursts;
};

struct obj {
	Process *process;
	Obj *next;
	Obj *prev;
};

struct list {
	Obj *first;
};

struct schedulerInfo{
	int processId;
	CpuBurst *cpu_done;
	IOBurst *io_done;
	char status;

};

struct processor
{
	Process *executingProcess;
	int currentCpuBurstOver;
	int idle;	
};


void setProcesses(FILE *file, Process*);
void setProcess(Process *, char *ch, int attrIndex, int);
char *setInput(char *buffer,int in_tam);
void schedulerStart(Process *, int);
int hasProcess(List *, List *, List *, Processor *);
void setNewQueue(List *, Process *, int);
void addOrderedByAdmission(List *, Obj *);
void admitProcess(List *, List *, int );
void scheduleProcess(Processor *, List*, List*);
void addOrderedByPriority(List *, Obj *);
void addToQueue(List *, Obj *);
void aging(List *);
void preemptionCheck(Processor *processor, List *readyQueue);
void preemptionExec(Processor *processor, List *readyQueue);
void execProcess(Processor *);
void execIO(List *waitQueue);
void checkIOFinished(List *waitQueue,List *readyQueue);
void displayQueueStatus(List * , List *);


int main(int argc, char **argv){	
	int i,j,SIZE = atoi(argv[2]);
	Process *processes;
	processes = (Process *) malloc(sizeof(Process) * SIZE);
	FILE *file;
	file = fopen(argv[1], "r");
	setProcesses(file, processes);
	fclose(file);
	schedulerStart(processes, SIZE);
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
				if(p->cpuBursts == NULL){
					p->cpuBursts = obj;
				} else {
					aux = p->cpuBursts;
					while(aux->next != NULL)
						aux = aux->next;
					aux->next = obj;
				}
			} else {
				IOBurst *obj, *aux;
				obj = (IOBurst *) malloc(sizeof(IOBurst));
				obj->burstTime = atoi(ch);
				obj->next = NULL;
				if(p->ioBursts == NULL){
					p->ioBursts = obj;
				} else {
					aux = p->ioBursts;
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

void schedulerStart(Process *processes, int size){
	List *newQueue, *readyQueue, *waitQueue;
	Processor *processor;
	newQueue = (List*) calloc(1,sizeof(List));
	readyQueue = (List *) calloc(1,sizeof(List));
	waitQueue = (List *) calloc(1, sizeof(List));
	processor = (Processor*) calloc(1,sizeof(Processor));
	processor->idle = ON; //iniciando processador com idle on
	processor->currentCpuBurstOver = OFF;
	int adTime, agingTime = 10, currentTime = 0;
	setNewQueue(newQueue, processes, size);
	while(hasProcess(newQueue,readyQueue,waitQueue, processor)){
		admitProcess(newQueue,readyQueue, currentTime);
		scheduleProcess(processor, readyQueue, waitQueue);
		execProcess(processor);
		execIO(waitQueue);
		checkIOFinished(waitQueue,readyQueue);
		if( currentTime % agingTime == 0 ){
			aging(readyQueue);
		}
		preemptionCheck(processor,readyQueue);
		displayQueueStatus(waitQueue, readyQueue);
		currentTime++;
	}
	free(newQueue);
	free(processor);
	free(readyQueue);
	free(waitQueue);
}

int hasProcess(List *newQueue, List *readyQueue, List *waitQueue, Processor *processor){
	if(newQueue->first || readyQueue->first || waitQueue->first || processor->executingProcess){
		return 1;
	} else {
		return 0;
	}
}

void setNewQueue(List *n, Process *p, int size){
	int i;
	for(i=0 ; i<size ; i++){
		Obj *obj;
		obj = (Obj*) calloc(1,sizeof(Obj));
		obj->process = &p[i];
		addOrderedByAdmission(n,obj);
	}
}

void addOrderedByAdmission(List *someQueue, Obj *obj){

	if(someQueue->first == NULL){
		someQueue->first = obj;
	}else {
		Obj *aux;
		aux = someQueue->first;
		while(aux->next != NULL && aux->process->admission <= obj->process->admission)
			aux = aux->next;
		if(someQueue->first == aux && aux->process->admission > obj->process->admission){
			obj->next = someQueue->first;
			someQueue->first = obj;
			obj->next->prev = obj;
		} else if(aux->process->admission > obj->process->admission && (aux->next != NULL || aux->next == NULL)){
			obj->next = aux;
			aux->prev->next = obj;
			obj->prev = aux->prev;
			aux->prev = obj;
		} else {
			aux->next = obj;
			obj->prev = aux;
		}
	}
}

void admitProcess(List *newQueue, List *readyQueue, int currentTime){
	Obj *obj;
	obj = newQueue->first;
	while(obj){
		if(obj->process->admission == currentTime){
			newQueue->first = obj->next;
			newQueue->first ? newQueue->first->prev = NULL : NULL;
			obj->next = NULL;
			addOrderedByPriority(readyQueue, obj);
			obj = newQueue->first;
		} else {
			obj = NULL;
		}
	}
}

void addOrderedByPriority(List *someQueue, Obj *obj){
	Obj *aux;
	if(someQueue->first == NULL){
		someQueue->first = obj;
	}else {
		aux = someQueue->first;
		while(aux->next != NULL && aux->process->priority <= obj->process->priority)
			aux = aux->next;
		if(someQueue->first == aux && aux->process->priority > obj->process->priority){
			obj->next = someQueue->first;
			someQueue->first = obj;
			obj->next->prev = obj;
		} else if(aux->process->priority > obj->process->priority && (aux->next != NULL || aux->next == NULL)){
			obj->next = aux;
			aux->prev->next = obj;
			obj->prev = aux->prev;
			aux->prev = obj;
		} else {
			aux->next = obj;
			obj->prev = aux;
		}
	}
}

void scheduleProcess(Processor *p, List *readyQueue, List *waitQueue){
	if(p->idle){
		if(readyQueue->first){
			Obj *obj;
			obj = readyQueue->first;
			readyQueue->first = obj->next;
			readyQueue->first ? readyQueue->first->prev = NULL : NULL ;
			obj->next = NULL;
			p->executingProcess = obj->process;
			p->idle = OFF;
		}
	} else if(p->currentCpuBurstOver){
		Process *process = p->executingProcess;
		CpuBurst *cpuBurst = process->cpuBursts;
		process->cpuBursts = cpuBurst->next;
		cpuBurst->next = NULL;
		free(cpuBurst);
		Obj *obj;
		obj = (Obj *) calloc(1,sizeof(Obj));
		obj->process = process;
		addToQueue(waitQueue, obj);
		p->currentCpuBurstOver = OFF;
		p->executingProcess = NULL;
		p->idle = ON;
	}
}

void addToQueue(List *someQueue, Obj *obj){
	if(!someQueue->first){
		someQueue->first = obj;
	}else {
		Obj *aux;
		aux = someQueue->first;
		while(aux->next)
			aux = aux->next;
		aux->next = obj;
		obj->prev = aux;
	}
}

void execProcess(Processor *processor){
	if(processor->executingProcess && !processor->currentCpuBurstOver){
		printf("\nprocesso %d em execução tempo de CPU %d\n\n",processor->executingProcess->id, processor->executingProcess->cpuBursts->burstTime);
		if (processor->executingProcess->cpuBursts->burstTime <= 0){
			processor->currentCpuBurstOver = ON;
		} else {
			processor->executingProcess->cpuBursts->burstTime -= 1;
		}
	}
}

void aging(List *readyQueue){
	if(readyQueue->first){
		Obj *aux;
		aux = readyQueue->first;
		while(aux != NULL){
			if(aux->process->priority > 0 ) {
				aux->process->priority -= 1;
			}
			aux = aux->next;
		}
	}
}

void preemptionCheck(Processor *processor, List *readyQueue){
	if (readyQueue->first)
	{
		if (processor->executingProcess && processor->executingProcess->priority > readyQueue->first->process->priority)
		{
			preemptionExec(processor, readyQueue);
		}
	}
}

void preemptionExec(Processor *processor, List *readyQueue){
	Process *processOut;
	Obj *obj;
	obj = readyQueue->first;
	if(obj != NULL){
		readyQueue->first = obj->next;
		readyQueue->first ? readyQueue->first->prev = NULL : NULL;
		obj->next = NULL;
		processOut = processor->executingProcess;
		processor->executingProcess = obj->process;
		obj->process = processOut;
		printf("Preempção: trocando processo %d por processo %d\n",processOut->id, processor->executingProcess->id);
		addOrderedByPriority(readyQueue,obj);
	}
}

void execIO(List *waitQueue){
	if(waitQueue->first){
		Obj *aux;
		aux = waitQueue->first;
		while(aux != NULL){
			aux->process->ioBursts->burstTime -= 1;
			aux = aux->next;
		}
	}
}

void checkIOFinished(List *waitQueue,List *readyQueue){
	if(waitQueue->first){
		Obj *obj, *aux;
		IOBurst *ioBurst;
		obj = waitQueue->first;
		while(obj != NULL ){
			if(waitQueue->first == obj && obj->process->ioBursts->burstTime == 0){
				waitQueue->first = obj->next;
				waitQueue->first ? waitQueue->first->prev= NULL : NULL;
				obj->next = NULL;
				ioBurst = obj->process->ioBursts;
				obj->process->ioBursts = ioBurst->next;
				free(ioBurst);
				if(obj->process->cpuBursts){
					addOrderedByPriority(readyQueue, obj);
				} else {
					printf("processo %d concluído\n", obj->process->id);
				}
				obj = waitQueue->first;
			} else if(obj->process->ioBursts->burstTime == 0){
				obj->prev->next = obj->next;
				obj->next ? obj->next->prev = obj->prev : NULL;
				aux = obj->next;
				obj->next = NULL;
				obj->prev = NULL;
				ioBurst = obj->process->ioBursts;
				obj->process->ioBursts = ioBurst->next;
				free(ioBurst);
				if(obj->process->cpuBursts){
					addOrderedByPriority(readyQueue, obj);
				} else {
					printf("processo %d concluído\n", obj->process->id);
				}
				obj = aux;
			} else {
				obj = obj->next;
			}
		}
	}
}

void displayQueueStatus(List *waitQueue, List *readyQueue){
	Obj *waitObj, *readyObj;
	waitObj = waitQueue->first;
	readyObj = readyQueue->first;
	printf("\nFila de Espera:");
	while(waitObj != NULL){
		printf(" Id:%d P:%d |",waitObj->process->id, waitObj->process->priority);
		waitObj = waitObj->next;
	}
	printf("\nFila de Prontos:");
	while(readyObj != NULL){
		printf(" Id:%d Prioridade:%d |",readyObj->process->id, readyObj->process->priority);
		readyObj = readyObj->next;
	}
	printf("\n");
}

