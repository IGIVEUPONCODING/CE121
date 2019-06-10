#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <wiringPi.h>
#include <errno.h>
#include <wiringPiI2C.h>


int processUSB(int nsample, char *channel_data, char *usb_data);
