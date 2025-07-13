#include <unistd.h>
#include <poll.h>
#include <sys/time.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>

#include "inputs.h"

void takeInput(char* shm)
{
	int timeoutMs = 5000;
	char inputDev[] = "/dev/input/event0";
	int st;
	int ret;
	struct pollfd fds[1];

	shm[rightIndex] = 0;
	shm[leftIndex] = 0;

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

	int exitOnKeyPressCount = 10;

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

		exitOnKeyPressCount--;
		// if(exitOnKeyPressCount<1)
			// break;
	}

	close(fds[0].fd);
	return;
}