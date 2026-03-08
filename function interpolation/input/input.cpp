#include "input.h"
#include <cmath>
#include <iostream>

double analytical_function(double x, int k)
{
    switch (k)
    {
        case 1: return 1.0;
        case 2: return x;
        case 3: return x*x;
        case 4: return x*x*x;
        case 5: return x*x*x*x;
        case 6: return std::exp(x);
        case 7: return 1.0/(25.0*x*x + 1.0);
        default:
            std::cerr << "Unknown function\n";
            return 0.0;
    }
}

void generate_input(double a, double b, int n, int k, double* x, double* f)
{
    double h = (b - a)/(n - 1);
    for (int i = 0; i < n; ++i)
    {
        x[i] = a + i*h;
        f[i] = analytical_function(x[i], k);
    }
}