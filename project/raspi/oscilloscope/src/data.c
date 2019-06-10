/*Xingyu Zhou
 * xzhou45@ucsc.edu
 * Final Project
 * This program process the data coming from
 * the PSoC
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
#include "usbcomm.h"
#include "data.h"




int processUSB(int nsample, char *channel_data, char *usb_data)
{	
	//printf("Processing\n");
	//Keep filling up the the buffer until certain sample is reached. 
	int j = 0;
	for (int i = 0; i<nsample; i++)
	{
		//printf("Moving Data.\n");
		//printf("Current Data %d\n", i);
		channel_data[i] = usb_data[j];
		j++;
		if(j >= 64){
			j = 0;
		}
	}
}
