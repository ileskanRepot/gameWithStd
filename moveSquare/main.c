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
  struct Square *sharedSquare =
      (struct Square *)mmap(NULL, sizeof(struct Square), PROT_READ | PROT_WRITE,
                            MAP_SHARED | MAP_ANON, -1, 0);

  pid_t mainFork = fork();
  if (mainFork < 0) {
    perror("Fork failed");
    exit(1);
  }
  if (mainFork == 0) {
    int draw = 1;
    drawLoop(sharedSquare, draw);
    exit(0);
  }

  pid_t physicsFork = fork();
  if (physicsFork < 0) {
    perror("Fork failed");
    exit(1);
  }
  if (physicsFork == 0 && mainFork != 0) {
    physicsLoop(inputMemory, sharedSquare);
    exit(0);
  }

  if (physicsFork != 0 && mainFork != 0) {
    inputLoop(inputMemory);
    exit(0);
  }
  return 0;
}
