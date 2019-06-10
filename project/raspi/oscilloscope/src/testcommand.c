#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]){
	int i;
	for(i=0;i<argc;i++){
		if(i == 1){
			printf("you only got 1 argument.\n");
		}
		else if(i == 2){
			printf("select trigger mode\n");
		}
	}
}
