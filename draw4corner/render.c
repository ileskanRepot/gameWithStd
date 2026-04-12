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

void sortTriangleCorners(struct Triangle *triangle) {
  for (int ii = 0; ii < 3; ii++) {
    for (int jj = ii + 1; jj < 3; jj++) {
      if (triangle->corners[ii].yy > triangle->corners[jj].yy) {
        swap(&triangle->corners[ii].xx, &triangle->corners[jj].xx);
        swap(&triangle->corners[ii].yy, &triangle->corners[jj].yy);
      }
    }
  }

  return;
}

void drawLine(unsigned char *frameBuffer, struct Point p0, struct Point p1) {
  struct Line line;
  if (p0.xx > p1.xx) {
    swap(&p0.xx, &p1.xx);
    swap(&p0.yy, &p1.yy);
  }
  twoPointToLine(&p0, &p1, &line);

  if (absMy(line.kk) > 1) {
    if (p0.yy > p1.yy) {
      swap(&p0.xx, &p1.xx);
      swap(&p0.yy, &p1.yy);
    }

    for (int yy = p0.yy; yy < p1.yy; yy++) {
      int xx = ((double)yy - line.bb) * line.oneOverkk;
      int pos = yy;
      pos *= WIDTH;
      pos += xx;
      pos *= PIXEL_SIZE;

      frameBuffer[pos + 0] = 255;
      frameBuffer[pos + 1] = 0;
      frameBuffer[pos + 2] = 255;
    }

  } else {
    for (int xx = p0.xx; xx < p1.xx; xx++) {
      int pos = line.kk * xx + line.bb;
      pos *= WIDTH;
      pos += xx;
      pos *= PIXEL_SIZE;

      frameBuffer[pos + 0] = 255;
      frameBuffer[pos + 1] = 0;
      frameBuffer[pos + 2] = 255;
    }
  }
}

void draw4corners(unsigned char *frameBuffer, struct FourCorner *fourCorner) {
  fourCorner->drawTriangles[0].corners[0] = fourCorner->corners[0];
  fourCorner->drawTriangles[0].corners[1] = fourCorner->corners[1];
  fourCorner->drawTriangles[0].corners[2] = fourCorner->corners[2];

  fourCorner->drawTriangles[1].corners[0] = fourCorner->corners[0];
  fourCorner->drawTriangles[1].corners[1] = fourCorner->corners[3];
  fourCorner->drawTriangles[1].corners[2] = fourCorner->corners[2];

  drawTriangle(frameBuffer, &fourCorner->drawTriangles[0], 0, 0, 100);
  drawTriangle(frameBuffer, &fourCorner->drawTriangles[1], 0, 0, 100);

  for (int ii = 0; ii < 4; ii++) {
    drawLine(frameBuffer, fourCorner->corners[ii],
             fourCorner->corners[(ii + 1) % 4]);
    // printf("(%.02f, %.02f) -> (%.02f, %.02f)\n", fourCorner->corners[ii].xx,
    //        fourCorner->corners[ii].yy, fourCorner->corners[(ii + 1) % 4].xx,
    //        fourCorner->corners[(ii + 1) % 4].yy);
  }
}

void twoPointToLine(struct Point *p0, struct Point *p1, struct Line *line) {
  if (p0->xx != p1->xx) {
    line->kk = (p0->yy - p1->yy) / (p0->xx - p1->xx);
    line->oneOverkk = 1.0 / line->kk;
    line->vert = 0;
  } else {
    line->kk = 1000 * 1000 * 1000;
    line->oneOverkk = 1.0 / line->kk;
    line->vert = 1;
  }
  line->bb = p0->yy - line->kk * p0->xx;
}

void drawTriangle(unsigned char *frameBuffer, struct Triangle *triangle,
                  unsigned char col0, unsigned char col1, unsigned char col2) {
  sortTriangleCorners(triangle);

  struct Line l0t2, l0t1, l1t2;
  twoPointToLine(&triangle->corners[0], &triangle->corners[1], &l0t1);
  twoPointToLine(&triangle->corners[0], &triangle->corners[2], &l0t2);
  twoPointToLine(&triangle->corners[1], &triangle->corners[2], &l1t2);

  for (int yy = triangle->corners[0].yy; yy < triangle->corners[1].yy; yy++) {
    int xxStart =
        l0t2.vert ? triangle->corners[0].xx : (yy - l0t2.bb) * l0t2.oneOverkk;
    int xxEnd =
        l0t1.vert ? triangle->corners[0].xx : (yy - l0t1.bb) * l0t1.oneOverkk;
    if (xxStart > xxEnd) {
      int tmp = xxStart;
      xxStart = xxEnd;
      xxEnd = tmp;
    }
    for (int xx = xxStart; xx < xxEnd; xx++) {
      int pos = yy;
      pos *= WIDTH;
      pos += xx;
      pos *= PIXEL_SIZE;

      frameBuffer[pos + 0] = col0;
      frameBuffer[pos + 1] = col1;
      frameBuffer[pos + 2] = col2;
    }
  }

  for (int yy = triangle->corners[1].yy; yy < triangle->corners[2].yy; yy++) {
    int xxStart =
        l0t2.vert ? triangle->corners[0].xx : (yy - l0t2.bb) * l0t2.oneOverkk;
    int xxEnd =
        l1t2.vert ? triangle->corners[1].xx : (yy - l1t2.bb) * l1t2.oneOverkk;
    if (xxStart > xxEnd) {
      int tmp = xxStart;
      xxStart = xxEnd;
      xxEnd = tmp;
    }
    for (int xx = xxStart; xx < xxEnd; xx++) {
      int pos = yy;
      pos *= WIDTH;
      pos += xx;
      pos *= PIXEL_SIZE;

      frameBuffer[pos + 0] = col0;
      frameBuffer[pos + 1] = col1;
      frameBuffer[pos + 2] = col2;
    }
  }

  // for (int ii = 0; ii < 3; ii++) {
  //   int pos = triangle->corners[ii].yy;
  //   pos *= WIDTH;
  //   pos += (int)triangle->corners[ii].xx;
  //   pos *= PIXEL_SIZE;

  //   frameBuffer[pos + 0] = 255;
  //   frameBuffer[pos + 1] = 0;
  //   frameBuffer[pos + 2] = 255;
  //   // printf("xx: %.02f yy: %.02f\n", triangle->corners[ii].xx,
  //   //        triangle->corners[ii].yy);
  // }

  return;
}

void writeToScreen(int fd, unsigned char *frameBuffer) {
  lseek(fd, 0, SEEK_SET);
  int ret = write(fd, frameBuffer, BUFF_SIZE);
  if (ret < 0) {
    perror("Could not write to frame buffer");
    exit(1);
  }
}

int drawLoop(struct FourCorner *sharedFourCorner, int draw) {
  int screenFd = open("/dev/fb0", O_WRONLY);
  if (screenFd < 0) {
    perror("Could not open frame buffer");
    exit(1);
  }

  char record = 0;

  unsigned char *frameBuffer = malloc(BUFF_SIZE);
  struct FourCorner *fourCorner = sharedFourCorner;

  struct timeval start, stop1, stop2;

  char up = 0, down = 0, left = 0, right = 0, running = 1, tooSlow = 0;
  while (running) {
    gettimeofday(&start, NULL);

    blackBox(frameBuffer);

    // drawSquare(frameBuffer, square, 0, 0, 255);
    // drawTriangle(frameBuffer, &triangle[0], 0, 0, 255);
    // drawTriangle(frameBuffer, &triangle[1], 255, 0, 255);
    draw4corners(frameBuffer, fourCorner);
    if (draw) {
      writeToScreen(screenFd, frameBuffer);
    }
    gettimeofday(&stop1, NULL);

    uint64_t delta_us1 = (stop1.tv_sec - start.tv_sec) * 1000000 +
                         (stop1.tv_usec - start.tv_usec);
    int sleepTime = 1000 * 1000 / FPS - delta_us1;
    printf("Took: '%ld'\n", delta_us1);
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
