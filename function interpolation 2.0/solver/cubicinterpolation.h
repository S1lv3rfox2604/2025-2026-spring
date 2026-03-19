#ifndef CUBIC_INTERPOLATION_H
#define CUBIC_INTERPOLATION_H

void cubic_interpolation(int n, const double* x, const double* f, double* m);

double evaluate_cubic(int n, const double* x, const double* f, const double* m, double point);

#endif