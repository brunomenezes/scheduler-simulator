#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void setProcesses(FILE *file, void *p);
void setProcess(void *p, char *ch, int attrIndex);

typedef struct process {
		char name[10];
		unsigned int priority, admission, id, *cpuBurst, *ioBurst, ioIndex, cpuIndex;
}Process;

int main(int argc, char **argv){	
	int BURST_TIMES = atoi(argv[3]);
	int i,size = atoi(argv[2]);
	struct Process processes[size];
	FILE *file;
	file = fopen(argv[1], "r");
	for(i = 0 ; i < size ; i++){
		processes[i].ioBurst = (int *) malloc(sizeof(int)*BURST_TIMES);
		processes[i].cpuBurst = (int *) malloc(sizeof(int)*BURST_TIMES);
		processes[i].cpuBurstBegin = processes[i].cpuBurst;
		processes[i].ioBurstBegin = processes[i].ioBurst;
	}
	setProcesses(file, processes);
	return 0;
}


void setProcesses(FILE *file, void *p){
	Process *p = (struct Process *) p;
	int buffer = 15;
	char input[buffer];
	char index = 0;
	int processIndex = 0;
	int attrIndex = 0
	char ch;
	ch = fgetc(file);
	while(ch != EOF){
		if(ch != '\t' && ch != '\n'){
			info[index] = ch;
			index++;
		} elseif(ch == '\n'){
			processIndex++;
			attrIndex = 0;
			index = 0;
		} else {
			setProcess(&processList[processIndex], input, attrIndex)
			buffer = 0;
		}
	
		ch = fgetc(file);
	}
	fclose(file);
}  

void setProcess(void *p, Char *ch, attrIndex) {
	struct Process *p = (struct Process *) p;
	switch(attrIndex){
		case 0:
			p->admission = atoi(*p);
			break;
		case 1: 
			strcpy(p->name, ch);
			break;
		case 2:
			p->priority = atoi(*p)
		default:
			if(attrIndex % 2 == 1){

			}
			break;
	}
}
