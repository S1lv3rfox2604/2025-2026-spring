#include <iostream>
#include <cmath>
#include <chrono>

#include "input/input.h"
#include "solver/chebyshev.h"
#include "solver/cubicinterpolation.h"

// Для графического режима
#include <QApplication>
#include "gui/mainwindow.h"  // Изменён путь

int main(int argc, char* argv[])
{
    if (argc == 7)   // консольный режим: a b n k method point
    {
        double a = std::stod(argv[1]);
        double b = std::stod(argv[2]);
        int n = std::stoi(argv[3]);
        int k = std::stoi(argv[4]);
        int method = std::stoi(argv[5]);
        double point = std::stod(argv[6]);

        if (n < 2) { std::cerr << "n must be >= 2\n"; return 1; }
        if (k < 1 || k > 7) { std::cerr << "k must be 1..7\n"; return 1; }

        double* x = new double[n];
        double* f = new double[n];
        double* coeffs = new double[n];

        double approx, exact = analytical_function(point, k);

        if (method == 1)
        {
            chebyshev_interpolation(a, b, n, k, x, coeffs);
            approx = evaluate_chebyshev(n, x, coeffs, point);
        }
        else if (method == 2)
        {
            if (n < 4) { std::cerr << "n must be >= 4 for cubic interpolation\n"; return 1; }
            generate_input(a, b, n, k, x, f);
            double* m = new double[n];
            cubic_interpolation(n, x, f, m);
            approx = evaluate_cubic(n, x, f, m, point);
            delete[] m;
        }
        else
        {
            std::cerr << "Unknown method\n";
            delete[] x; delete[] f; delete[] coeffs;
            return 1;
        }

        std::cout << "Error at point " << point << " = " << std::fabs(approx - exact) << "\n";

        delete[] x; delete[] f; delete[] coeffs;
        return 0;
    }
    else if (argc == 6)   // графический режим: a b n k method
    {
        double a = std::stod(argv[1]);
        double b = std::stod(argv[2]);
        int n = std::stoi(argv[3]);
        int k = std::stoi(argv[4]);
        int method = std::stoi(argv[5]);

        QApplication app(argc, argv);
        MainWindow win(a, b, n, k, method);
        win.show();
        return app.exec();
    }
    else
    {
        std::cerr << "Usage: " << argv[0] << " a b n k method [point]\n"
                  << "  with 6 arguments: start GUI\n"
                  << "  with 7 arguments: console mode, print error at point\n";
        return 1;
    }
}