#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(){
	char *a = (char *) calloc(10,sizeof(char));
	char b[]= "brunolimademenezes";
	strcpy(a,b);
	printf("%d\n",strlen(a) );
	puts(a);
	return 0;
	
}
