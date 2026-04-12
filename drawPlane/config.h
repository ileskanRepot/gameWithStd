#pragma once

const int WIDTH = 1920;
const int HEIGHT = 1080;
#define PIXEL_SIZE (4)
#define BUFF_SIZE (WIDTH * HEIGHT * PIXEL_SIZE)
#define FPS (100) // Frames per second
#define PPS (100) // OPhysics per second

const int KEY_COUNT = 5;
enum charKeyCodes {
  rightCode = 32,
  leftCode = 30,
  upCode = 17,
  downCode = 31,
  exitCode = 16
};
enum listIndex {
  rightIndex = 0,
  leftIndex = 1,
  upIndex = 2,
  downIndex = 3,
  exitIndex = 4
};
