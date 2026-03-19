#ifndef CHEBYSHEV_H
#define CHEBYSHEV_H

void chebyshev_interpolation(double a, double b, int n, int k, double* x, double* coeffs);

double evaluate_chebyshev(int n, const double* x, const double* coeffs, double point);

#endif