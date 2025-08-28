#pragma once
#include "config.h"
#include <bits/types/struct_timeval.h>

struct Square {
  double xx;
  double yy;
  double hh;
  double ww;

  double rot;
};

struct Point {
  double xx;
  double yy;
};

struct inputEvent {
  struct timeval time;
  unsigned short type;
  unsigned short code;
  unsigned int value;
};
