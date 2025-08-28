#include "inputs.h"

void inputLoop(char *inputMemory) {
  int timeoutMs = 5000;
  char inputDev[] = "/dev/input/event0";
  int st;
  int ret;
  struct pollfd fds[1];

  inputMemory[rightIndex] = 0;
  inputMemory[leftIndex] = 0;
  inputMemory[upIndex] = 0;
  inputMemory[downIndex] = 0;

  fds[0].fd = open(inputDev, O_RDONLY | O_NONBLOCK);

  if (fds[0].fd < 0) {
    printf("error unable open for reading '%s'\n", inputDev);
    return;
  }

  const int inputSize = sizeof(struct inputEvent);
  // unsigned char inputData[inputSize];
  struct inputEvent inputData[1];
  memset(inputData, 0, inputSize);

  fds[0].events = POLLIN;

  int running = 1;
  while (running) {
    ret = poll(fds, 1, timeoutMs);
    if (ret <= 0) {
      printf("timeout\n");
      continue;
    }
    if (!fds[0].revents) {
      printf("error\n");
      continue;
    }

    ssize_t r = read(fds[0].fd, inputData, inputSize);

    if (r < 0) {
      printf("error %d\n", (int)r);
      break;
    }
    // printf("total bytes read %d/%d\n",(int)r,inputSize);
    if ((inputData->type == 0 && inputData->code == 0 &&
         inputData->value == 0) ||
        (inputData->value == 2) || (inputData->type != 1)) {
      continue;
    }
    // printf("type=%hu code=%hu value=%u\n", inputData->type, inputData->code,
    // inputData->value);

    switch (inputData->code) {
    case rightCode:
      inputMemory[rightIndex] = inputData->value;
      break;
    case leftCode:
      inputMemory[leftIndex] = inputData->value;
      break;
    case upCode:
      inputMemory[upIndex] = inputData->value;
      break;
    case downCode:
      inputMemory[downIndex] = inputData->value;
      break;
    case exitCode:
      inputMemory[exitIndex] = inputData->value;
      running = !inputData->value;
      break;
    default:
      break;
    }
    memset(inputData, 0, inputSize);
  }

  close(fds[0].fd);
  return;
}