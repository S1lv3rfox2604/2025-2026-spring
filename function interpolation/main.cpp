#include <iostream>
#include <cmath>
#include <chrono>

#include "input/input.h"
#include "solver/chebyshev.h"
#include "solver/cubicinterpolation.h"
#include "output/output.h"

int main(int argc, char* argv[])
{
    if (argc != 6)
    {
        std::cerr << "Usage: ./main a b n k method\n";
        return 1;
    }

    double a = std::stod(argv[1]);
    double b = std::stod(argv[2]);
    int n = std::stoi(argv[3]);
    int k = std::stoi(argv[4]);
    int method = std::stoi(argv[5]);

    if (n < 2)
    {
        std::cerr << "n must be >= 2\n";
        return 1;
    }

    double* x = new double[n];
    double* f = new double[n];
    double* coeffs = nullptr; // для хранения коэффициентов (Чебышев) или производных (кубический)

    if (k>7){
        std::cerr << "k must be <=7\n";
        return 1;
    }

    generate_input(a, b, n, k, x, f);

    auto start = std::chrono::high_resolution_clock::now();

    if (method == 1)
    {
        // Чебышев
        coeffs = new double[n];
        chebyshev_interpolation(a, b, n, k, x, coeffs);
    }
    else if (method == 2)
    {
        // Кубическая интерполяция
        cubic_interpolation(n, x, f, coeffs);
    }
    else
    {
        std::cerr << "Unknown method\n";
        delete[] x; delete[] f;
        return 1;
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    std::cout << "Algorithm time: " << elapsed.count() << " seconds\n";

    print_result(a, b, n, k, x, f, coeffs, method);

    delete[] x;
    delete[] f;
    delete[] coeffs;

    return 0;
}