#pragma once
#include "config.h"
#include <bits/types/struct_timeval.h>

struct Point {
  double xx;
  double yy;
};

struct Line {
  double kk;
  double oneOverkk;
  double bb;
};

struct Color {
  unsigned char rr;
  unsigned char gg;
  unsigned char bb;
};

struct Square {
  double xx;
  double yy;
  double hh;
  double ww;

  double rot;
};

struct FourCorner {
  struct Point corners[4];
};

/*
 * In trinagle the corners should be sorted by x value
 */
struct Triangle {
  struct Point corners[3];
};

struct inputEvent {
  struct timeval time;
  unsigned short type;
  unsigned short code;
  unsigned int value;
};
