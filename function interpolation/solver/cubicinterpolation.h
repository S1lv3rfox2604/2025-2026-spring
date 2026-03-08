#ifndef CUBIC_INTERPOLATION_H
#define CUBIC_INTERPOLATION_H

void cubic_interpolation(int n, double* x, double* f, double*& m);

double evaluate_cubic(int n, double* x, double* f, double* m, double point);

#endif