/*Xingyu Zhou
 * xzhou45@ucsc.edu
 * Final Project
 * This file parses the command line arguments
 * 
 * */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <wiringPi.h>
#include <errno.h>
#include <wiringPiI2C.h>
#include "cmdparse.h"

int parseCommand (struct parsingCommand *Command, int argc, char *argv[]){
	//Set all the default values
	Command->mode = trigger;
	Command->triglvl = 2500;
	Command->trigslp = pos;
	Command->samprate = 1;
	Command->channel = 1;
	Command->xs = 1000;
	Command->ys = 1000;
	for(int i = 0; i<argc; i++){
		//Make sure there are enough Commandline argument being inputed
		if(argc % 2 == 0){
			printf("Not enough argument!\n");
			return -1;
		}
		//check for the mode
		if(strcmp(argv[i], "-m") == 0){
			if(strcmp(argv[i+1],"trigger") == 0){
				Command->mode = trigger;
			}else if(strcmp(argv[i+1],"free_running") == 0){
				Command->mode = free_running;
			}else{ 
				printf("Error! No such mode!\n");
				return -1;
			}		
		}
		//check for the trigger level
		else if(strcmp(argv[i],"-t") == 0){
			int x = atoi(argv[i+1]);
			/*Since a non numeric input will return a 0 with atoi() function,
			 *it's possible that this input will change my current triglvl to 0.
			 *The condition below make sure that the triglvl will only be 0 when
			 *an actual number "0" is inputed.*/
			if(x == 0 && strcmp(argv[i+1],"0") != 0){
				printf("Error! Numeric values only!\n");
				return -1;
			}
			//Make sure the step size is 100.
			if(x%100 !=0){
				printf("Error! Triglvl must be in step of 100!\n");
				return -1;
			}
			//make sure the triglvl is within the specific range
			else if(x<0 || x >5000){
				printf("Error! Triglvl must be in the range from 0 to 5000!\n");
				return -1;
			}
			//set the triglvl in the step of 100 from 0 to 5000;
			else{
				Command->triglvl = x;
			}
		}
		//check for trigger slope
		else if(strcmp(argv[i],"-s") == 0){
			if(strcmp(argv[i+1],"pos") == 0)
				Command->trigslp = pos;
			if(strcmp(argv[i+1],"neg") == 0)
				Command->trigslp = neg;
			else{
				printf("Error! Please select the correct trigger slope!(pos or neg)\n");
				return -1;
			}
		}
		//Check for sampling rate
		else if(strcmp(argv[i],"-r") == 0){
			int x = atoi(argv[i+1]);
			if(x != 1 && x != 10 && x != 20 && x != 50 && x != 100){
				printf("Error! Sample rate must be 1k, 10k, 20k, 50k, or 100k.\n");
				return -1;
			} else {
				Command->samprate = x;
			}
		}
		//check for the channel
		else if(strcmp(argv[i],"-c") == 0){
			if(strcmp(argv[i+1],"1") == 0)
				Command->channel = 1;
			else if(strcmp(argv[i+1],"2") == 0)
				Command->channel = 2;
			else{
				printf("Error! Select correct channel (1 or 2)!\n");
				return -1;
			}
		}
		//check for the xscale
		else if(strcmp(argv[i],"-x") == 0){
			int x = atoi(argv[i+1]);
			if(x != 100 && x != 500 && x != 1000 && x != 2000 && x != 5000 && x != 10000 && x != 50000 && x!= 100000){
				printf("Error! scale must be 100, 500, 1000, 20000 ,50000, 100000, 500000 or 1000000.\n");
				return -1;
			}
			else {
				Command->xs = x;
			}
		}
		//check for the yscale
		else if(strcmp(argv[i],"-y") == 0){
			int x = atoi(argv[i+1]);
			if(x != 100 && x != 500 && x != 1000 && x != 2000 && x != 2500){
				printf("Error! scale must be 1, 5, 10, 20 or 25.\n");
				return -1;
			}
			else {
				Command->ys = x;
			}
		}
	}
}
	
	
		
		
			
		
		
