
/*Xingyu Zhou
 * xzhou45@ucsc.edu
 * Final Project
 * This is function handles the USB as well as 
 * the i2c data transfer.
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

libusb_device_handle* dev;


int usbinit()
{
    libusb_init(NULL); // Initialize the LIBUSB library
    // Open the USB device (the Cypress device has
    // Vendor ID = 0x04B4 and Product ID = 0x8051)
    dev = libusb_open_device_with_vid_pid(NULL, 0x04B4, 0x8051); 
    
    if (dev == NULL){
	perror("device not found\n");
	return -1;
    }
  
    // Reset the USB device.
    // This step is not always needed, but clears any residual state from
    // previous invocations of the program.
    if (libusb_reset_device(dev) != 0){
        perror("Device reset failed\n");
	return -1;
    } 

    // Set configuration of USB device
    if (libusb_set_configuration(dev, 1) != 0){
        perror("Set configuration failed\n");
	return -1;
    } 


    // Claim the interface.  This step is needed before any I/Os can be
    // issued to the USB device.
    if (libusb_claim_interface(dev, 0) !=0){
          perror("Cannot claim interface");
	  return -1;
      }
}

int usbread (char *rx_data, char *rx2_data)
{
    int rcvd_bytes1; // Bytes received
    int rcvd_bytes2;
    int return_val;    
    // Perform the IN transfer (from device to host).
    // This will read the data back from the device.
    return_val = libusb_bulk_transfer(dev, // Handle for the USB device
				    (0x01 | 0x80), // Address of the Endpoint in USB device
				    // MS bit nust be 1 for IN transfers
				    rx_data, // address of receive data buffer
				    64, // Size of data buffer
				    &rcvd_bytes1, // Number of bytes actually received
				    0 // Timeout in milliseconds (0 to disable timeout)
				    );
    /*if (return_val == 0){
	printf("%d bytes received in buffer1\n", rcvd_bytes1);
	for (int i=0; i<rcvd_bytes1; i++){
	    printf("%02x ", rx_data[i]);
	    if (i % 16 == 15) printf("\n");
	    }
	printf("\n");
    }*/
    // Perform the IN transfer (from device to host).
    // This will read the data back from the device.
    return_val = libusb_bulk_transfer(dev, // Handle for the USB device
				    (0x02 | 0x80), // Address of the Endpoint in USB device
				    // MS bit nust be 1 for IN transfers
				    rx2_data, // address of receive data buffer
				    64, // Size of data buffer
				    &rcvd_bytes2, // Number of bytes actually received
				    0 // Timeout in milliseconds (0 to disable timeout)
				    );
    /*if (return_val == 0){
	printf("%d bytes received in buffer2\n", rcvd_bytes2);
	for (int i=0; i<rcvd_bytes2; i++){
	    printf("%02x ", rx2_data[i]);
	    if (i % 16 == 15) printf("\n");
	}
	printf("\n");
    }*/
}
int i2cinit(int fd){
    fd = wiringPiI2CSetup(I2C_SLAVE_ADDR);
    if (fd == -1){
	perror("device not found\n");
	return -1;
    }
}

int i2cread (int command, int *pot1, int *pot2)
{
    // Write command to PSoC
    int fd = wiringPiI2CSetup(I2C_SLAVE_ADDR);
    wiringPiI2CWrite(fd, command);
    // Read potentiometer data
    *pot1 = wiringPiI2CRead(fd);
    *pot2 = wiringPiI2CRead(fd);
}


