#include "render.h"
#include "config.h"
#include "misc.h"
#include "structs.h"

void blackBox(unsigned char *frameBuffer) {
  memset(frameBuffer, 0, WIDTH * HEIGHT * PIXEL_SIZE);
}

void drawSquare(unsigned char *frameBuffer, struct Square *square,
                unsigned char col0, unsigned char col1, unsigned char col2) {
  for (int yy = 0; yy < square->hh; yy++) {
    for (int xx = 0; xx < square->ww; xx++) {
      int pos = ((((int)square->yy) + yy) * WIDTH + (((int)square->xx) + xx)) *
                PIXEL_SIZE;
      frameBuffer[pos + 0] = col0;
      frameBuffer[pos + 1] = col1;
      frameBuffer[pos + 2] = col2;
    }
  }
}

void drawLine(unsigned char *frameBuffer, struct Point point1,
              struct Point point2) {
  struct Point startPoint;
  struct Point endPoint;

  if (absMy(point1.xx - point2.xx) < absMy(point1.yy - point2.yy)) {
    startPoint.xx = point1.yy < point2.yy ? point1.xx : point2.xx;
    startPoint.yy = point1.yy < point2.yy ? point1.yy : point2.yy;

    endPoint.xx = point1.yy < point2.yy ? point2.xx : point1.xx;
    endPoint.yy = point1.yy < point2.yy ? point2.yy : point1.yy;

    double oneOverkk =
        (endPoint.xx - startPoint.xx) / (endPoint.yy - startPoint.yy);
    double bb = (endPoint.yy) - (1.0 / oneOverkk * endPoint.xx);
    double zeroOffset = 0;
    if (endPoint.yy - startPoint.yy == 0.0 ||
        endPoint.xx - startPoint.xx == 0.0) {
      bb = endPoint.xx;
      oneOverkk = 0;
      zeroOffset = endPoint.xx;
    }
    // printf("kk: %lf bb: %lf\n", oneOverkk, bb);
    for (unsigned int yy = startPoint.yy; yy < endPoint.yy; yy++) {
      unsigned int xxCur = (yy - bb) * oneOverkk + zeroOffset;
      unsigned int yyCur = yy;
      // printf("xx: %d yy: %d\n", xxCur, yyCur);
      // break;

      unsigned int pos = ((yyCur)*WIDTH + (xxCur)) * PIXEL_SIZE;

      frameBuffer[pos + 0] = 255;
      frameBuffer[pos + 1] = 0;
      frameBuffer[pos + 2] = 255;
      frameBuffer[pos + 3] = 0;
    }
  } else {
    startPoint.xx = point1.xx < point2.xx ? point1.xx : point2.xx;
    startPoint.yy = point1.xx < point2.xx ? point1.yy : point2.yy;

    endPoint.xx = point1.xx < point2.xx ? point2.xx : point1.xx;
    endPoint.yy = point1.xx < point2.xx ? point2.yy : point1.yy;

    double kk = (endPoint.yy - startPoint.yy) / (endPoint.xx - startPoint.xx);
    double bb = startPoint.yy;
    if (endPoint.yy - startPoint.yy == 0.0 ||
        endPoint.xx - startPoint.xx == 0.0) {
      bb = (endPoint.yy) - (kk * endPoint.xx);
    }

    for (int xx = startPoint.xx; xx < endPoint.xx; xx++) {
      unsigned int xxCur = xx;
      unsigned int yyCur = xx * kk + bb;
      unsigned int pos = ((yyCur)*WIDTH + (xxCur)) * PIXEL_SIZE;

      frameBuffer[pos + 0] = 255;
      frameBuffer[pos + 1] = 0;
      frameBuffer[pos + 2] = 255;
      frameBuffer[pos + 3] = 0;
    }
  }
}

void writeToScreen(int fd, unsigned char *frameBuffer) {
  lseek(fd, 0, SEEK_SET);
  int ret = write(fd, frameBuffer, BUFF_SIZE);
  if (ret < 0) {
    perror("Could not write to frame buffer");
    exit(1);
  }
}

int drawLoop(struct Square *sharedSquare, int draw) {
  int screenFd = open("/dev/fb0", O_WRONLY);
  if (screenFd < 0) {
    perror("Could not open frame buffer");
    exit(1);
  }

  char record = 0;

  unsigned char *frameBuffer = malloc(BUFF_SIZE);
  struct Square *square = sharedSquare;

  struct timeval start, stop1, stop2;

  char up = 0, down = 0, left = 0, right = 0, running = 1;
  while (running) {
    gettimeofday(&start, NULL);

    blackBox(frameBuffer);

    drawSquare(frameBuffer, square, 0, 0, 255);
    if (draw) {
      writeToScreen(screenFd, frameBuffer);
    }
    gettimeofday(&stop1, NULL);

    uint64_t delta_us1 = (stop1.tv_sec - start.tv_sec) * 1000000 +
                         (stop1.tv_usec - start.tv_usec);
    int sleepTime = 1000 * 1000 / FPS - delta_us1 - 100;
    if (sleepTime < 0) {
      printf("Too slow");
    } else {
      usleep(sleepTime);
    }
    gettimeofday(&stop2, NULL);
    uint64_t delta_us2 = (stop2.tv_sec - start.tv_sec) * 1000000 +
                         (stop2.tv_usec - start.tv_usec);
  }

  free(frameBuffer);
  close(screenFd);
  return 0;
}
