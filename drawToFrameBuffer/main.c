#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define WIDTH 1920
#define HEIGHT 1080
#define PIXEL_SIZE 4
#define BUFF_SIZE WIDTH*HEIGHT*PIXEL_SIZE

int mapInt(int inp, int input_start, int input_end, int output_start, int output_end){
	return output_start + (output_end - output_start) * (inp - input_start) / (input_end - input_start);
}

void colorGradient(char *frameBuffer){
	for (int yy = 0; yy < HEIGHT; yy++){
		for (int xx = 0; xx < WIDTH; xx++){
			int pos = (yy*WIDTH+xx)*PIXEL_SIZE;
			frameBuffer[pos +0] = mapInt(xx, 0, WIDTH, 0, 255);
			frameBuffer[pos +1] = 0;
			frameBuffer[pos +2] = mapInt(yy, 0, HEIGHT, 0, 255);
			frameBuffer[pos +3] = 0;
		}
	}
}

void writeToScreen(int fd, char* frameBuffer){
	int ret = write(fd, frameBuffer, BUFF_SIZE);
	if (ret < 0){
		perror("Could not write to frame buffer");
		exit(1);
	}
}

int main(){
	int screenFd = open("/dev/fb0", O_WRONLY);
	if (screenFd < 0){
		perror("Could not open frame buffer");
		exit(1);
	}

	char *frameBuffer = malloc(BUFF_SIZE);
	colorGradient(frameBuffer);
	writeToScreen(screenFd, frameBuffer);

	free(frameBuffer);
	close(screenFd);
}
