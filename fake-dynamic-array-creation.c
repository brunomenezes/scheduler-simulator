#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define IDLE_ON 1;
typedef struct x Obj;
typedef struct y List;

struct x
{
	Obj *next;
	Obj *prev;
	int number;
};

struct y
{
	Obj *first;
	Obj *last;
};

Obj* createNode(int number);
void addToList(List *, int number);
void displayList(List *);

int main(){
	List *list;
	list = (List*) calloc(1,sizeof(List));
	int n,i ; 
	printf("tamanho da lista : ");
	scanf("%d", &n);
	for(i=0 ; i < n ; i++){
		addToList(list,(i+1));
	}
	displayList(list);
	return 0;
	
}

Obj* createNode(int number){
	Obj *obj;
	obj = (Obj *) malloc(sizeof(Obj));
	obj->number = number;
	obj->next = NULL;
	obj->prev = NULL;
	return obj;

}

void addToList(List *l, int number){
	Obj *obj = createNode(number);
	Obj *aux;
	if(!l->first){
		l->first = obj;
	} else {
		aux = l->first;
		while(aux->next)
			aux = aux->next;
		aux->next = obj;
		obj->prev = aux;
	}
	l->last = obj;
}

void displayList(List *l){
	Obj *obj, *toRelease;
	obj = l->first;
	toRelease = obj ;
	l->first = NULL;
	while(obj != NULL){
		printf("number : %d\n",obj->number);
		obj = obj->next;
		free(toRelease);
		toRelease = obj;
	}
}
