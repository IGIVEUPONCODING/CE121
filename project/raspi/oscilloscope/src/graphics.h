/*Xingyu Zhou
 * xzhou45@ucsc.edu
 * Final Project
 * This is the header file of all my drawing functions
 * */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <shapes.h>
#include <fontinfo.h>
#include <math.h>

typedef struct{
  VGfloat x, y;
} data_point;

void grid(VGfloat x, VGfloat y, int nx, int ny, int w, int h);
void background(int w, int h, int xdiv, int ydiv, int margin);
void showscale(int xscale, int yscale, int xpos, int ypos, VGfloat tcolor[4]);
void processSamples(char *data, int nsamples, int xstart, int xfinish, float yscale, data_point *point_array);
void plotWave(data_point *data, int nsamples, int yoffset, VGfloat linecolor[4]);
void waituntil(int endchar);


