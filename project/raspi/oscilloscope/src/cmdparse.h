/*Xingyu Zhou
 * xzhou45@ucsc.edu
 * Final Project
 * This is the header file of my parsing program
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

enum scope_mode{
	free_running,
	trigger
};

enum trigger_slope{
	pos,
	neg
};

struct parsingCommand
{
	int mode;
	int triglvl;
	int trigslp;
	int samprate;
	int channel;
	int xs;
	int ys;
};

/*declare the parsing function*/
int parseCommand (struct parsingCommand *Command, int argc, char *argv[]);
