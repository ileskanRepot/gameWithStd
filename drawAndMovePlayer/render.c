#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

#include "render.h"
// #include "inputs.h"

#define WIDTH 1920
#define HEIGHT 1080
#define PIXEL_SIZE 4
#define BUFF_SIZE WIDTH*HEIGHT*PIXEL_SIZE

struct Square {
	unsigned int xx;
	unsigned int yy;
	unsigned int hh;
	unsigned int ww;
	double rot;
};

struct Point {
   unsigned int xx;
   unsigned int yy;
};

int mapInt(int inp, int input_start, int input_end, int output_start, int output_end){
	return output_start + (output_end - output_start) * (inp - input_start) / (input_end - input_start);
}

void blackBox(char *frameBuffer){
	for (int yy = 0; yy < HEIGHT; yy++){
		for (int xx = 0; xx < WIDTH; xx++){
			int pos = (yy*WIDTH+xx)*PIXEL_SIZE;
			frameBuffer[pos +0] = 0;
			frameBuffer[pos +1] = 0;
			frameBuffer[pos +2] = 0;
			frameBuffer[pos +3] = 0;
		}
	}
}
void drawSquare(char *frameBuffer, struct Square square, unsigned char col0, unsigned char col1, unsigned char col2){
	blackBox(frameBuffer);

	for (int yy = 0; yy < square.hh;yy++){
		for (int xx = 0; xx < square.ww; xx++){
			int xxCur = cos(square.rot)*xx - sin(square.rot)*yy;
			int yyCur = sin(square.rot)*xx + cos(square.rot)*yy;
			unsigned int pos = ((square.yy+yyCur)*WIDTH + (square.xx+xxCur))*PIXEL_SIZE;

			/*
			frameBuffer[pos +0] = col0;
			frameBuffer[pos +1] = col1;
			frameBuffer[pos +2] = col2;
			frameBuffer[pos +3] = 0;
			*/

			frameBuffer[pos +0] = 255;
			frameBuffer[pos +1] = 255;
			frameBuffer[pos +2] = 255;
			frameBuffer[pos +3] = 0;
		}
	}
}

void drawLine(char *frameBuffer, struct Point point1, struct Point point2){
	// blackBox(frameBuffer);

	double kk = ((double)(point2.yy - point1.yy))/((double)(point2.xx - point1.xx));
	double bb = point1.yy;
	if (kk != 0){
	 	bb = ((double)point2.yy)-(kk*(double)point2.xx);
	}
	// printf("%lf %lf %d\n", kk, bb, point2.xx);
	for (int ii = point1.xx; ii < point2.xx; ii++){
		unsigned int xxCur = ii;
		unsigned int yyCur = ii*kk+bb;
		unsigned int pos = ((yyCur)*WIDTH + (xxCur))*PIXEL_SIZE;

		frameBuffer[pos +0] = 255;
		frameBuffer[pos +1] = 0;
		frameBuffer[pos +2] = 255;
		frameBuffer[pos +3] = 0;
	}
}

void writeToScreen(int fd, char* frameBuffer){
	lseek(fd, 0, SEEK_SET);
	int ret = write(fd, frameBuffer, BUFF_SIZE);
	if (ret < 0){
		perror("Could not write to frame buffer");
		exit(1);
	}
}

int mainRender(char* shm){
	int screenFd = open("/dev/fb0", O_WRONLY);
	if (screenFd < 0){
		perror("Could not open frame buffer");
		exit(1);
	}

	char *frameBuffer = malloc(BUFF_SIZE);
	struct Square square = { 
		.xx = 500,
		.yy = 500,
		.ww = 300,
		.hh = 300
	};

	struct Point point1 = {
		.xx = 100,
		.yy = 100,
	};
	struct Point point2 = {
		.xx = 1000,
		.yy = 100,
	};
	blackBox(frameBuffer);
	unsigned char cols[3] = {0, 0, 0};
	char change = 0;
	char dir = 1;
	
	while (1) {
		// drawLine(frameBuffer, point1, point2);
		if (shm[leftIndex]){
			square.xx -= speed;
			if (square.xx < 0)
				square.xx = 0;
		}
		if (shm[rightIndex]){
			square.xx += speed;
			if (square.xx >= max)
				square.xx = max-1;
		}
		if (shm[upIndex]){
			square.yy -= speed;
			if (square.yy < 0)
				square.yy = 0;
		}
		if (shm[downIndex]){
			square.yy += speed;
			if (square.yy >= max)
				square.yy = max-1;
		}

		drawSquare(frameBuffer, square, 255, 0, 255);

		usleep(100*1000);
		writeToScreen(screenFd, frameBuffer);
	}

	free(frameBuffer);
	close(screenFd);
}
