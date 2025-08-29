#include "misc.h"

int mapInt(int inp, int input_start, int input_end, int output_start,
           int output_end) {
  return output_start + (output_end - output_start) * (inp - input_start) /
                            (input_end - input_start);
}
double sign(double num) {
  if (num > 0.0)
    return 1.0;
  if (num < 0.0)
    return -1.0;
  return 0.0;
}

double absMy(double num) { return sign(num) * num; }
