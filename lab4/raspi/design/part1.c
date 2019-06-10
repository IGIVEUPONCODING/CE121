/*Xingyu Zhou
 * Lab4 Part4
 * Raspberry Cpde
 * */

#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>
#include<termios.h>
#include<wiringPi.h>

#define BAUDRATE B115200 //UART transmit speed
#define LED_PIN 1
int count = 0;
float cheatcode = 0;
int main (int argc, char * argv[]){
	wiringPiSetup();
	pinMode (LED_PIN, PWM_OUTPUT);
	struct termios serial;
	char* UART = "/dev/serial0";
	char receive[100];
	int fd = open(UART, O_RDWR | O_NOCTTY | O_NDELAY);	
	if (fd == -1){
		perror(UART);
		return -1;
	}

	if(tcgetattr(fd,&serial)<0){
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
		for(int i = 0; i < 100; i++){
			int rdcount = read(fd, receive, sizeof(receive));
			if(rdcount < -1){
				perror("FATAL!!!!");
			}
			//Scale Up my maximum reading to 1023
			cheatcode = (receive[0] * 4) * 1.48;
			pwmWrite(LED_PIN, cheatcode);
		}
	}
	close(fd);
}
