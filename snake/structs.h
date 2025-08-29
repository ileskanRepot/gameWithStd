#pragma once
#include "config.h"
#include <bits/types/struct_timeval.h>

struct Point {
  double xx;
  double yy;
};

struct Color {
  unsigned char rr;
  unsigned char gg;
  unsigned char bb;
};

struct inputEvent {
  struct timeval time;
  unsigned short type;
  unsigned short code;
  unsigned int value;
};

struct Square {
  double xx;
  double yy;
  double hh;
  double ww;

  struct Color color;

  double rot;
};