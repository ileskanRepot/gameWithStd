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

void swap(double *num0, double *num1) {
  double tmp = *num0;
  *num0 = *num1;
  *num1 = tmp;
}

double vecLen3d(struct Point3d *point3d) {
  return sqrt(point3d->xx * point3d->xx + point3d->yy * point3d->yy +
              point3d->zz * point3d->zz);
}