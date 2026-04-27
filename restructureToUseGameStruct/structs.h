#pragma once
#include "config.h"
#include <bits/types/struct_timeval.h>

struct Point2d {
  double xx;
  double yy;
};

struct Point3d {
  double xx;
  double yy;
  double zz;
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
  struct Point2d corners[3];
};

/*
 * From corner 0 there is lines to corners 3 and 1
 * From corner 1 there is lines to corners 0 and 2
 * ...
 */
struct FourCorner {
  struct Point2d corners[4];
  struct Triangle drawTriangles[2];
};

struct Plane {
  struct Point3d corners[4];
};

struct Cube {
  struct Point3d pos;
  struct Point3d size;

  struct Square drawSquare[6];
  char isShownPlane[6];
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

struct Player {
  struct Point3d pos;
  struct Point3d dir;
};

struct Game {
  unsigned char running;
  struct Player player;
  struct Point3d point2d;
  struct Plane plane;
  char draw;
};
