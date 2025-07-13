#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

#include "structs.h"
#include "render.h"
#include "config.h"


int mapInt(int inp, int input_start, int input_end, int output_start, int output_end){
	return output_start + (output_end - output_start) * (inp - input_start) / (input_end - input_start);
}


int main(){
	int screenFd = open("/dev/fb0", O_WRONLY);
	if (screenFd < 0){
		perror("Could not open frame buffer");
		exit(1);
	}

	char *frameBuffer = malloc(BUFF_SIZE);
	struct Square square = { 
		.xx = 500,
		.yy = 500,
		.ww = 100,
		.hh = 100
	};


	struct Point points[4];
	points[0].xx = 0;
	points[0].yy = 0;

	points[1].xx = WIDTH-100;
	points[1].yy = 100;

	// printf("%lf %lf %d %d\n", points[0].xx, points[0].yy, points[1].xx, points[1].yy);

	points[2].xx = WIDTH-100;
	points[2].yy = HEIGHT-100;

	points[3].xx = 100;
	points[3].yy = HEIGHT-100;

	int dirX = 1;
	int dirY = 1;
	
	int range = 1;
	for (int ii = 0; ii < range; ii++){
		blackBox(frameBuffer);

		// drawLine(frameBuffer, points[0], points[1]);
		drawLine(frameBuffer, points[1], points[2]);
		// drawLine(frameBuffer, points[2], points[3]);
		// drawLine(frameBuffer, points[3], points[0]);

		// drawSquare(frameBuffer, square, 255, 0, 255);

		if (square.xx + square.ww >= WIDTH) {
			dirX = -1;
			square.xx = WIDTH - square.ww;
		} else if (square.xx <= 0) {
			dirX = 1;
			square.xx = 0;
		}
		if (square.yy + square.hh >= HEIGHT) {
			dirY = -1;
			square.yy = HEIGHT - square.hh;
		} else if (square.yy <= 0) {
			dirY = 1;
			square.yy = 0;
		}


		usleep(1*1000);
	 	square.xx += 5 *dirX;
	 	square.yy += 5 *dirY;

		writeToScreen(screenFd, frameBuffer);
	}

	free(frameBuffer);
	close(screenFd);
}
