#ifndef INPUTS_H
#define INPUTS_H

struct inputEvent {
	struct timeval time;
	unsigned short type;
	unsigned short code;
	unsigned int value;
};

enum charKeyCodes {rightCode = 32, leftCode = 30, upCode = 17, downCode = 31, exitCode = 16};
enum listIndex {rightIndex = 0, leftIndex = 1, upIndex = 2, downIndex = 3, exitIndex = 4};

void takeInput(char* shm);

#endif