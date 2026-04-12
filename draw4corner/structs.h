#pragma once
#include "config.h"
#include <bits/types/struct_timeval.h>

struct GameState {
  unsigned char running;
};

struct Point {
  double xx;
  double yy;
};

struct Line {
  double kk;
  double oneOverkk;
  unsigned char vert;
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

struct Triangle {
  struct Point corners[3];
};

/*
 * From corner 0 there is lines to corners 3 and 1
 * From corner 1 there is lines to corners 0 and 2
 * ...
 */
struct FourCorner {
  struct Point corners[4];
  struct Triangle drawTriangles[2];
};

/*
 * In trinagle the corners should be sorted by x value
 */

struct inputEvent {
  struct timeval time;
  unsigned short type;
  unsigned short code;
  unsigned int value;
};
