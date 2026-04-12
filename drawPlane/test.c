#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/time.h>

#include "render.h"

#define WIDTH (1920)
#define HEIGHT (1080)
#define PIXEL_SIZE (4)
#define BUFF_SIZE (WIDTH*HEIGHT*PIXEL_SIZE)

int main(int argc, char *argv[]){
	unsigned char *frameBuffer = malloc(BUFF_SIZE);
	struct timeval start, stop;
	unsigned int range = 1000;
	gettimeofday(&start, NULL);
	int kk = 0;
	for (unsigned int ii = 0; ii < range; ii++) {
		blackBox(frameBuffer);
	}
	gettimeofday(&stop, NULL);

	uint64_t delta_us = (stop.tv_sec - start.tv_sec) * 1000000 + (stop.tv_usec - start.tv_usec);
	printf("took %lu us \n", delta_us/range);
}
