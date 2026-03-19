#include "output.h"
#include "../solver/chebyshev.h"
#include "../solver/cubicinterpolation.h"
#include <cmath>
#include <iostream>

double test_function(double x, int k)
{
    switch (k)
    {
        case 1: return 1.0;
        case 2: return x;
        case 3: return x*x;
        case 4: return x*x*x;
        case 5: return x*x*x*x;
        case 6: return std::exp(x);
        case 7: return 1.0 / (1.0 + 25*x*x);
        default:
            std::cerr << "Unknown function\n";
            return 0.0;
    }
}

void print_result(double a, double b, int n, int k, double* x, double* f, double* coeffs, int method)
{
    double max_error = 0.0;

    for (int i = 0; i <= 1000; ++i)
    {
        double point = a + (b - a)*i/1000.0;
        double approx;

        if (method == 1)
            approx = evaluate_chebyshev(n, x, coeffs, point);
        else if (method == 2)
            approx = evaluate_cubic(n, x, f, coeffs, point);
        else
        {
            std::cerr << "Unknown method\n";
            return;
        }

        double exact = test_function(point, k);
        double error = std::fabs(approx - exact);
        if (error > max_error)
            max_error = error;
    }

    std::cout << "Max error = " << max_error << "\n";
}