#include "physics.h"
#include "structs.h"

int physicsLoop(char *inputMemory, struct Square *sharedSquare,
                struct Triangle *sharedTriangle) {
  //   unsigned char *frameBuffer = malloc(BUFF_SIZE);
  struct Triangle *triangle = sharedTriangle;
  triangle->corners[0].xx = 100;
  triangle->corners[0].yy = 100;

  triangle->corners[1].xx = 1632;
  triangle->corners[1].yy = 376;

  triangle->corners[2].xx = 1400;
  triangle->corners[2].yy = 854;

  struct Square *square = sharedSquare;
  square->xx = 500;
  square->yy = 500;
  square->ww = 100;
  square->hh = 100;

  struct Point max = {.xx = WIDTH, .yy = HEIGHT};

  const double baseSpeed = 15.0;
  const double bothDirSpeed = 1.0 / sqrt(2);
  double speedXX = 0, speedYY = 0;
  double speedDrop = 1.0;
  struct timeval start, stop1, stop2;

  char up = 0, down = 0, left = 0, right = 0, running = 1;
  while (running) {
    gettimeofday(&start, NULL);
    up = inputMemory[upIndex] != 0;
    down = inputMemory[downIndex] != 0;
    left = inputMemory[leftIndex] != 0;
    right = inputMemory[rightIndex] != 0;
    running = inputMemory[exitIndex] == 0;

    if (left) {
      if (up ^ down) {
        speedXX = -baseSpeed * bothDirSpeed;
      } else {
        speedXX = -baseSpeed;
      }
    }
    if (right) {
      if (up ^ down) {
        speedXX = baseSpeed * bothDirSpeed;
      } else {
        speedXX = baseSpeed;
      }
    }
    if (up) {
      if (left ^ right) {
        speedYY = baseSpeed * bothDirSpeed;
      } else {
        speedYY = baseSpeed;
      }
    }
    if (down) {
      if (left ^ right) {
        speedYY = -baseSpeed * bothDirSpeed;
      } else {
        speedYY = -baseSpeed;
      }
    }
    if (up && down) {
      speedYY = 0;
    }
    if (left && right) {
      speedXX = 0;
    }

    triangle->corners[0].xx += speedXX;
    triangle->corners[0].yy -= speedYY;
    speedXX -= sign(speedXX) * speedDrop;
    speedYY -= sign(speedYY) * speedDrop;
    speedXX = absMy(speedXX) < speedDrop ? 0 : speedXX;
    speedYY = absMy(speedYY) < speedDrop ? 0 : speedYY;

    if (triangle->corners[0].yy >= max.yy)
      triangle->corners[0].yy = max.yy - 1;
    if (triangle->corners[0].xx >= max.xx)
      triangle->corners[0].xx = max.xx - 1;
    if (triangle->corners[0].yy < 0)
      triangle->corners[0].yy = 0;
    if (triangle->corners[0].xx < 0)
      triangle->corners[0].xx = 0;

    gettimeofday(&stop1, NULL);

    uint64_t delta_us1 = (stop1.tv_sec - start.tv_sec) * 1000000 +
                         (stop1.tv_usec - start.tv_usec);
    useconds_t sleepTime = 1000 * 1000 / PPS - delta_us1 - 100;
    usleep(sleepTime);
    gettimeofday(&stop2, NULL);
    uint64_t delta_us2 = (stop2.tv_sec - start.tv_sec) * 1000000 +
                         (stop2.tv_usec - start.tv_usec);
  }

  return 0;
}
