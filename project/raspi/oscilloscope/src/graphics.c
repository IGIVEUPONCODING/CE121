/*Xingyu Zhou
 * xzhou45@ucsc.edu
 * Final Project
 * This program handles all the drawing
 * 
 * */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <shapes.h>
#include <fontinfo.h>
#include <math.h>
#include "graphics.h"


void waituntil(int endchar){
	int key;
	for(;;){
		key = getchar();
		if(key == endchar || key == '\n'){
			break;
		}
	}
}
//Draw the grid lines
void grid(VGfloat x, VGfloat y, int nx, int ny, int w, int h)
{
	VGfloat ix, iy;
	Stroke(128,128,128,0.5);
	StrokeWidth(2);
	//draw horizontal grid lines
	for (ix = x; ix <= x + w; ix += w/nx){
		Line(ix, y, ix, y + h);
	}
	//draw vertical grid line
	for (iy = y; iy <= y + h; iy += h/ny){
		Line(x, iy, x + w, iy);
	}
}
//Draw out the background of the Oscilloscope
void background(int w, int h, int xdiv, int ydiv, int margin)
{
	VGfloat x1 = margin;
	VGfloat y1 = margin;
	VGfloat x2 = w - 2*margin;
	VGfloat y2 = h - 2*margin;
	
	Background(128, 128, 128); //set background color
	Stroke(201, 204, 204, 1);
	StrokeWidth(1);
	
	Fill(44, 77, 120, 1);
	Rect(10, 10, w-20, h-20);
	grid(x1, y1, xdiv, ydiv, x2, y2); //Draw grid lines
}

// X and Y scale display
void showscale(int xscale, int yscale, int xpos, int ypos, VGfloat tcolor[4])
{
	char str[100];
	
	setfill(tcolor);
	if (xscale >= 1000){
		sprintf(str, "X scale = %0d ms/div", xscale/1000);
	}
	else{
		sprintf(str, "X scale = %0d us/div", xscale);
	}
	Text(xpos, ypos, str, SansTypeface, 18);
	sprintf(str, "Y scale = %3.2f V/div", yscale * 0.001);
	Text(xpos, ypos-50, str, SansTypeface, 18);
}

void processSamples(char *data, int nsamples, int xstart, int xfinish, float yscale, data_point *point_array)
{
  VGfloat x1, y1;
  data_point p;

  for (int i=0; i< nsamples; i++){
    x1 = xstart + (xfinish-xstart)*i/nsamples;
    //Bring up the scale 10 times more
    y1 = data[i] * 5 * 10 * yscale/256;
    p.x = x1;
    p.y = y1;
    point_array[i] = p;
    }
}


//Plot the waveforms
void plotWave(data_point *data, int nsamples, int yoffset, VGfloat linecolor[4])
{
  data_point p;
  VGfloat x1, y1, x2, y2;

  Stroke(linecolor[0], linecolor[1], linecolor[2], linecolor[3]);
  StrokeWidth(4);

  p = data[0];
  x1 = p.x;
  y1 = p.y + yoffset;

  for (int i=1; i< nsamples; i++){
    p = data[i];
    x2 = p.x;
    y2 = p.y + yoffset;
    Line(x1, y1, x2, y2);
    x1 = x2;
    y1 = y2;
    }
}



	
	
	

