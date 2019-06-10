myscope: main.o cmdargs.o usbcomm.o graphics.o data.o
	gcc -g -o myscope main.o cmdargs.o usbcomm.o graphics.o data.o  -lshapes -lm -lusb-1.0 -lwiringPi

main.o: main.c graphics.h cmdparse.h usbcomm.h data.h
	gcc -g -c main.c -I /opt/vc/include -I /usr/include -I/usr/include/libusb-1.0 -lshapes -lm -lusb-1.0 -lwiringPi

cmdargs.o: cmdargs.c cmdparse.h
	gcc -g -c cmdargs.c

usbcomm.o: usbcomm.c usbcomm.h
	gcc -g -c usbcomm.c -I/usr/include/libusb-1.0 -lshapes -lm -lusb-1.0 -lwiringPi

graphics.o: graphics.c graphics.h
	gcc -g -c graphics.c -I /opt/vc/include -I /usr/include -lshapes

data.o: data.c data.h
	gcc -g -c data.c -I/usr/include/libusb-1.0 -lshapes -lm -lusb-1.0

clean: 
	rm *.o 


    

