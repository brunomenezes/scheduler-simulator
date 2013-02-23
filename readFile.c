#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void getProcess(FILE *file);

typedef struct process {
		unsigned int admission;
		char name[10];
		unsigned int priority;
		unsigned int *cpuBurst;
		unsigned int *ioBurst;
}Process;

int main(int argc, char **argv){	
	int BURST_TIMES = atoi(argv[2]);
	FILE *file;
	file = fopen(argv[1], "r");
	getProcess(file);
	return 0;
}


void getProcess(FILE *file, SIZE, BURST_TIMES){
	Process processList[SIZE];
	//*2 porque e a quantidade de rajada total, logo se passar 6 temos um array que terá 12 inteiros 6 para cpu e 6 para IO
	int burstTimes[BURST_TIMES*2];
	char info[15];
	char buffer = 0;
	int processIndex = 0;
	int infoIndex = 0
	char ch;
	ch = fgetc(file);
	while(ch != EOF){
		if(ch != '\t'){
			info[buffer] = ch;
			buffer++;
		} else {
			setProcess(&processList[processIndex], info, infoIndex)
			buffer = 0;
		}
		if(ch == '\n'){
			processIndex++;
			infoIndex++;
			buffer = 0;
		}
		ch = fgetc(file);
	}
	fclose(file);
}  

void setProcess(void *p, Char *ch, infoIndex) {
	Process *p = (struct Process *) p;
	switch(infoIndex){
		case 0:
			p->admission = atoi(*p);
			break;
		case 1: 
			strcpy(p->name, ch);
			break;
		case 2:
			p->priority = atoi(*p)
		default:
			
			break;
	}
}
