#pragma once

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

#include "config.h"
#include "structs.h"

void blackBox(unsigned char *frameBuffer);
void drawSquare(unsigned char *frameBuffer, struct Square *square, unsigned char col0, unsigned char col1, unsigned char col2);
void drawLine(unsigned char *frameBuffer, struct Point point1, struct Point point2);
void writeToScreen(int fd, char* frameBuffer);