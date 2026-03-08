#include "chebyshev.h"
#include "../input/input.h"
#include <cmath>

void chebyshev_interpolation(double a, double b, int n, int k, double* x, double* coeffs)
{
    double* f = new double[n];

    for (int i = 0; i < n; ++i)
    {
        double t = std::cos(M_PI*(2*i+1)/(2*n));
        x[i] = 0.5*(a+b) + 0.5*(b-a)*t;
        f[i] = analytical_function(x[i], k);
        coeffs[i] = f[i];
    }

    // Интерполяционный многочлен (форма Ньютона)
    for (int j = 1; j < n; ++j)
        for (int i = n-1; i >= j; --i)
            coeffs[i] = (coeffs[i] - coeffs[i-1]) / (x[i] - x[i-j]);

    delete[] f;
}

double evaluate_chebyshev(int n, double* x, double* coeffs, double point)
{
    double result = coeffs[n-1];
    for (int i = n-2; i >= 0; --i)
        result = result*(point - x[i]) + coeffs[i];
    return result;
}