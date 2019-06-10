/*Xingyu Zhou
 * xzhou45@ucsc.edu
 * Final Project
 * This is the header file containing
 * the usb and i2c functions.
 * */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <wiringPi.h>
#include <errno.h>
#include <libusb.h>
#include <wiringPiI2C.h>
#define  I2C_SLAVE_ADDR 0x53 // 7-bit I2C address of PSoC


int usbread (char *rx_data, char *rx2_data);
int i2cread (int command, int *pot1, int *pot2); 
int usbinit();
int i2cinit(int fd);
