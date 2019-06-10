/* Xingyu Zhou
 * xzhou45@ucsc.edu
 * Final Project
 * This program controls everything 
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
#include "graphics.h"
#include "usbcomm.h"
#include "data.h"
#define  I2C_SLAVE_ADDR 0x53 // 7-bit I2C address of PSoC

int main(int argc, char *argv[]){
	printf("Oscilloscope Starting:--------------------\n");
	struct parsingCommand test;
	int i = parseCommand(&test, argc, argv);
	if(test.mode == 1){
		printf("Mode: Trigger\n");
	}
	if(test.mode == 0){
		printf("Mode: Free\n");
	}
	printf("Trigger Level: %d\n", test.triglvl);
	if(test.trigslp == 0){
		printf("Trigger Slope: Positive\n");
	}
	if(test.trigslp == 1){
		printf("Trigger Slope: Negative\n");
	}
	printf("Sample Rate: %d\n", test.samprate);
	printf("Channel: %d\n", test.channel);
	printf("X Scale: %d\n", test.xs);
	printf("Y Scale: %d\n", test.ys);
	printf("------------------------------------------\n");
	//Data decleration
	unsigned int command;
	command = test.samprate;
	int pot1 = 0;
	int pot2 = 0;
	int fd;
	char rx_data[64];
	char rx2_data[64];
	int width, height;
	int margin = 10;
	int xdivisions = 10;
	int ydivisions = 8;
	char str[100];
	int xscale = test.xs;
	int yscale = test.ys;
	int sr = test.samprate;
	int nsample = xscale * sr / 100;
	printf("# of samples: %d\n", nsample);
	int xstart = margin;
	int xlimit = width - 2*margin;
	int ystart = margin;
	int ylimit = height - 2*margin;
	int trigval = test.triglvl*256/5000;
	
	int pixels_per_volt = (ylimit-ystart)*1000/(ydivisions*yscale);
	 
	char channel1_data[nsample*3]; // Data samples from Channel 1
	char channel2_data[nsample*3]; // Data samples from Channel 2
	//Allocating these two new arrays will stop my code from drawing the waveform.
	//I've tried malloc and it still doesn't work.	
	char channel1_data_new[nsample]; // Data samples from Channel 1
	char channel2_data_new[nsample]; // Data samples from Channel 2	
	data_point channel1_points[nsample];
	data_point channel2_points[nsample];
	


	
	saveterm();
	init(&width, &height);
	Start(width, height);
	rawterm();
	
	VGfloat textcolor[4] = {0, 200, 200, 0.5};
	VGfloat wave1color[4] = {240, 0, 0, 0.5}; // Color for displaying Channel 1 data
	VGfloat wave2color[4] = {200, 200, 0, 0.5}; // Color for displaying Channel 2 data
	while(1){
		i2cinit(fd);
		usbinit();
		//write command to PSoC such as changing sample rate.
		i2cread(command, &pot1, &pot2);
		//Used for index tracking
		int b = 0;
		int m = 0;
		//get the data from USB
		//Read twice to skip the first set of data that comes in.		
		usbread(rx_data,rx2_data);
		usbread(rx_data,rx2_data);
		//Processing the data and draw out the waveform.			
		processUSB(nsample*3, channel1_data, rx_data);
		processUSB(nsample*3, channel2_data, rx2_data);
		
		if(test.mode == 1){
		//positive slope
		if(test.trigslp == 0){
				//channel 1 trig detection
				if(test.channel == 1){
					for (int k = 1; k < nsample*3; k++){
						if(channel1_data[k-1] < trigval  && channel1_data[k] >= trigval){
							b++;
							break;
						} else {
							b++;
					}
				}
			}
			//channel 2 trig detection
			if(test.channel == 2){
				for (int k = 1; k < nsample*3; k++){
					if(channel2_data[k-1] < trigval && channel2_data[k] >= trigval){
						m++;
						break;
					} else {
						m++;
					}
				}
			}
		}
		
		//negative slope
		if(test.trigslp == 1){
			if(test.trigslp == 0){
				//channel 1 trig detection
				if(test.channel == 1){
					for(int k = 1; k < nsample*3; k++){
						if(channel1_data[k-1] > trigval && channel1_data[k] <= trigval){
							b++;
							break;
						} else {
							b++;
						}
					}
				}
				if(test.channel == 2){
					for (int k = 1; k < nsample*3; k ++){
						if(channel2_data[k-1] > trigval && channel2_data[k] <= trigval){
							m++;
							break;
						} else {
							m++;
						}
					}
				}
			}
		}
	}
							
		for(int k = 0; k < nsample; k++) {
			channel1_data_new[k] = channel1_data[k+b];
			channel2_data_new[k] = channel2_data[k+m];
		}
		//Processing data coming from the USB.
		processSamples(channel1_data_new, nsample, margin, width-2*margin, pixels_per_volt, channel1_points);
		processSamples(channel2_data_new, nsample, margin, width-2*margin, pixels_per_volt, channel2_points);
		background(width, height, xdivisions, ydivisions, margin);
		showscale(xscale, yscale, width-300, height-50, textcolor);
		//The Y offset is different EVERYTIME I pull from gitlab.
		plotWave(channel1_points, nsample, pot1*6, wave1color); //This is the yellow waveform
		plotWave(channel2_points, nsample, pot2*6, wave2color);	//This is the red waveform
		//command++;
		End();
		int key;
		key = getchar();
		if(key == 0x1b || key == '\n'){
			break;
		}
	}
	restoreterm();
	finish();
	return 0;
}

