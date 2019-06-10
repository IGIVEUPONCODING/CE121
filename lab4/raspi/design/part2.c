/*Xingyu Zhou
 * Lab4 Part2
 * Raspberry Code
 * The entire code is 
 * basically following the example
 * */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <wiringPi.h>
#include <errno.h>

#define BAUDRATE B115200 //UART transmit speed

int main (int argc, char* argv[]){
	wiringPiSetup();
	struct termios serial;
	char* UART="/dev/serial0";
	//char transmit [100];
	char receive [1000];
	int fd = open(UART, O_RDWR | O_NOCTTY | O_NDELAY);
	if (fd == -1){
		perror(UART);
		return -1;
	}
	if(tcgetattr(fd, &serial)<0){
		perror("Getting configuration");
		return -1;
	}
	serial.c_iflag = 0;
	serial.c_oflag = 0;
	serial.c_lflag = 0;
	serial.c_cflag = BAUDRATE | CS8 | CREAD | PARENB | PARODD;
	serial.c_cc[VMIN] = 0;
	serial.c_cc[VTIME] = 0;
	tcsetattr(fd, TCSANOW, &serial);
	
	for(;;){
		
		int rdcount = read(fd, receive, sizeof(receive));
		if(rdcount < -1){
			perror("FATAL");
			return -1;
			}
			//start transmit when theres actual stuff in rx
		if(rdcount >0){
			int wcount = write(fd, receive, rdcount);
			if(wcount < 0){
				if(errno != EAGAIN){
					wcount = write(fd, receive, rdcount);
					perror("Write");
					return -1;
				}
			}
		}
	}
	close(fd);
}
