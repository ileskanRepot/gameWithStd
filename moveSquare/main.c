#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <unistd.h>
#include <math.h>
#include <poll.h>
#include <string.h>

#include "structs.h"
#include "render.h"
#include "config.h"


int mapInt(int inp, int input_start, int input_end, int output_start, int output_end){
	return output_start + (output_end - output_start) * (inp - input_start) / (input_end - input_start);
}

void takeInput(char* shm) {
  int timeoutMs = 5000;
  char inputDev[] = "/dev/input/event0";
  int st;
  int ret;
  struct pollfd fds[1];

  shm[rightIndex] = 0;
  shm[leftIndex] = 0;
  shm[upIndex] = 0;
  shm[downIndex] = 0;

  fds[0].fd = open(inputDev, O_RDONLY|O_NONBLOCK);

  if(fds[0].fd<0)
  {
    printf("error unable open for reading '%s'\n",inputDev);
    return;
  }

  const int inputSize = sizeof(struct inputEvent);
  // unsigned char inputData[inputSize];
  struct inputEvent inputData[1];
  memset(inputData,0,inputSize);

  fds[0].events = POLLIN;

  while(1) {
    ret = poll(fds, 1, timeoutMs);
    if (ret <= 0){
      printf("timeout\n");
      continue;
    }
    if (!fds[0].revents) {
      printf("error\n");
      continue;
    }

    ssize_t r = read(fds[0].fd,inputData,inputSize);

    if(r<0)
    {
      printf("error %d\n",(int)r);
      break;
    }
    // printf("total bytes read %d/%d\n",(int)r,inputSize);
    if (
        (inputData->type == 0 && inputData->code == 0 && inputData->value == 0)
        || (inputData->value == 2)
        || (inputData->type != 1)
        // || (inputData->value == 2)
        ) {
      continue;
    }
    // printf("type=%hu code=%hu value=%u\n", inputData->type, inputData->code, inputData->value);

    switch (inputData->code) {
      case rightCode:
        shm[rightIndex] = inputData->value;
        break;
      case leftCode:
        shm[leftIndex] = inputData->value;
        break;
      case upCode:
        shm[upIndex] = inputData->value;
        break;
      case downCode:
        shm[downIndex] = inputData->value;
        break;
      default:
        break;
    }

    memset(inputData,0,inputSize);
  }

  close(fds[0].fd);
  return;
}


int mainPros(char* shm){
	int screenFd = open("/dev/fb0", O_WRONLY);
	if (screenFd < 0){
		perror("Could not open frame buffer");
		exit(1);
	}

	char record = 0;

	unsigned char *frameBuffer = malloc(BUFF_SIZE);
	struct Square *square;
	square->xx = 500;
	square->yy = 500;
	square->ww = 100;
	square->hh = 100;
	

	struct Point max = {
		.xx = WIDTH, 
		.yy = HEIGHT
	};

	double baseSpeed = 15.0;
	double bothDirSpeed = 1.0/sqrt(2);
	
	int range = 10000;
	char up = 0, down = 0, left = 0, right = 0; 
	for (int ii = 0; ii < range; ii++){
		up    = shm[upIndex]    != 0;
		down  = shm[downIndex]  != 0;
		left  = shm[leftIndex]  != 0;
		right = shm[rightIndex] != 0;


		if (left){
			if (up ^ down) {
				square->xx -= baseSpeed*bothDirSpeed;
			} else {
				square->xx -= baseSpeed;
			}
			if (square->xx < 0)
				square->xx = 0;
		}
		if (right){
			if (up ^ down) {
				square->xx += baseSpeed*bothDirSpeed;
			} else {
				square->xx += baseSpeed;
			}
			if (square->xx >= max.xx-square->ww)
				square->xx = max.xx-square->ww-1;
		}
		if (up){
			if (left ^ right) {
				square->yy -= baseSpeed*bothDirSpeed;
			} else {
				square->yy -= baseSpeed;
			}
			if (square->yy < 0)
				square->yy = 0;
		}
		if (down){
			if (left ^ right) {
				square->yy += baseSpeed*bothDirSpeed;
			} else {
				square->yy += baseSpeed;
			}
			if (square->yy >= max.yy-square->hh)
				square->yy = max.yy-square->hh-1;
		}

		blackBox(frameBuffer);
		drawSquare(frameBuffer, square, 0, 0, 255);
		writeToScreen(screenFd, frameBuffer);
		usleep(1*1000);
	}

	free(frameBuffer);
	close(screenFd);
}


int main(int argc, char *argv[]){
	char* shmem = (char*)mmap(
		NULL,
		8,
		PROT_READ | PROT_WRITE,
		MAP_SHARED | MAP_ANON,
		-1,
		0
	);

	pid_t pp = fork();
	if (pp < 0){
		perror("Fork failed");
		exit(1);
	}

	if (pp == 0){
    	// usleep(0.1 * 1000 * 1000);
    	// printf("%p %d\n", shmem, *(char*)shmem);
    	mainPros(shmem);
	}else{
    	// printf("%p %d\n", shmem, *(char*)shmem);
    	takeInput(shmem);
		// usleep(0.1 * 1000 * 1000);
	}
}
