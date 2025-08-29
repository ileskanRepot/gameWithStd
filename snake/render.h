#pragma once

#include <fcntl.h>
#include <math.h>
#include <poll.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include "config.h"
#include "misc.h"
#include "render.h"
#include "structs.h"

void blackBox(unsigned char *frameBuffer);
void drawSquare(unsigned char *frameBuffer, struct Square *square);
void drawLine(unsigned char *frameBuffer, struct Point point1,
              struct Point point2);
void writeToScreen(int fd, unsigned char *frameBuffer);
int drawLoop(struct Square **squares, int squareCount, int draw);