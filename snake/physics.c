#include "physics.h"
#include "config.h"

void initSquare(struct Square *square, double xx, double yy, double ww,
                double hh, struct Color col) {
  square->xx = xx;
  square->yy = yy;
  square->ww = ww;
  square->hh = hh;

  square->color.rr = col.rr;
  square->color.gg = col.gg;
  square->color.bb = col.bb;
};

int isInside(struct Square *sq1, double xx, double yy) {
  int retVal = ((sq1->xx < xx && xx < sq1->xx + sq1->ww) &&
                (sq1->yy < yy && yy < sq1->yy + sq1->hh));
  return retVal;
};

int checkColSquares(struct Square *sq1, struct Square *sq2) {
  int ret1 = isInside(sq1, sq2->xx, sq2->yy) ||
             isInside(sq1, sq2->xx + sq2->ww, sq2->yy) ||
             isInside(sq1, sq2->xx, sq2->yy + sq2->ww) ||
             isInside(sq1, sq2->xx + sq2->ww, sq2->yy + sq2->ww);
  int ret2 = isInside(sq2, sq1->xx, sq1->yy) ||
             isInside(sq2, sq1->xx + sq1->ww, sq1->yy) ||
             isInside(sq2, sq1->xx, sq1->yy + sq1->ww) ||
             isInside(sq2, sq1->xx + sq1->ww, sq1->yy + sq1->ww);
  return ret1 || ret2;
};

int physicsLoop(char *inputMemory, struct Square **squares) {
  struct Square *player = squares[0];
  struct Color colPlayer = {.rr = 255, .gg = 0, .bb = 255};
  initSquare(player, 0.0, 0.0, 100.0, 100.0, colPlayer);

  struct Square *box = squares[1];
  struct Color colBox = {.rr = 0, .gg = 255, .bb = 255};
  initSquare(box, 800.0, 800, 100.0, 100.0, colBox);

  struct Point max = {.xx = WIDTH, .yy = HEIGHT};

  const double baseSpeed = 15.0;
  const double jumpSpeed = 45.0;
  const double bothDirSpeed = 1.0 / sqrt(2);
  double speedXX = 0, speedYY = 0;
  double speedDrop = 1.0;
  struct timeval start, stop1, stop2;

  char onFloor = 0;

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
    if (up && onFloor) {
      onFloor = 0;
      speedYY = jumpSpeed;
    }
    if (down) {
      if (left ^ right) {
        speedYY = -baseSpeed * bothDirSpeed;
      } else {
        speedYY = -baseSpeed;
      }
    }
    if (up && down) {
      speedYY = GRAVITY;
    }
    if (left && right) {
      speedXX = 0;
    }

    player->xx += speedXX;
    if (checkColSquares(player, box)) {
      player->xx -= speedXX;
    }
    player->yy -= speedYY;
    if (checkColSquares(player, box)) {
      if (speedYY < 0) {
        onFloor = 1;
      }
      player->yy += speedYY;
    } else {
      onFloor = 0;
    }

    speedXX -= sign(speedXX) * speedDrop;
    speedYY -= sign(speedYY) * speedDrop;
    speedXX = absMy(speedXX) < speedDrop ? 0 : speedXX;
    speedYY = absMy(speedYY) < speedDrop ? 0 : speedYY;
    if (!onFloor)
      speedYY += GRAVITY;
    // printf("%f2", speedYY);

    if (player->yy >= max.yy - player->hh) {
      player->yy = max.yy - player->hh - 1;
      onFloor = 1;
    }
    if (player->xx >= max.xx - player->ww) {
      player->xx = max.xx - player->ww - 1;
    }
    if (player->xx < 0) {
      player->xx = 0;
    }
    if (player->yy < 0) {
      player->yy = 0;
    }

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
