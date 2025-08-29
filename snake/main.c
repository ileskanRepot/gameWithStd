#include <fcntl.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include "config.h"
#include "inputs.h"
#include "misc.h"
#include "physics.h"
#include "render.h"
#include "structs.h"

int main(int argc, char *argv[]) {
  char *inputMemory = (char *)mmap(NULL, KEY_COUNT, PROT_READ | PROT_WRITE,
                                   MAP_SHARED | MAP_ANON, -1, 0);
  struct Square *player =
      (struct Square *)mmap(NULL, sizeof(struct Square), PROT_READ | PROT_WRITE,
                            MAP_SHARED | MAP_ANON, -1, 0);
  struct Square *opsticle =
      (struct Square *)mmap(NULL, sizeof(struct Square), PROT_READ | PROT_WRITE,
                            MAP_SHARED | MAP_ANON, -1, 0);
  int squareCount = 2;
  struct Square **squares =
      (struct Square **)malloc(sizeof(struct Square *) * squareCount);
  squares[0] = player;
  squares[1] = opsticle;

  opsticle->xx = 500;
  opsticle->yy = 500;
  opsticle->hh = 500;
  opsticle->ww = 500;
  opsticle->color.rr = 0;
  opsticle->color.gg = 255;
  opsticle->color.bb = 255;

  pid_t mainFork = fork();
  if (mainFork < 0) {
    perror("Fork failed");
    exit(1);
  }
  if (mainFork == 0) {
    int draw = 1;
    drawLoop(squares, squareCount, draw);
    exit(0);
  }

  pid_t physicsFork = fork();
  if (physicsFork < 0) {
    perror("Fork failed");
    exit(1);
  }
  if (physicsFork == 0 && mainFork != 0) {
    physicsLoop(inputMemory, squares);
    exit(0);
  }

  if (physicsFork != 0 && mainFork != 0) {
    inputLoop(inputMemory);
    exit(0);
  }
  return 0;
}
