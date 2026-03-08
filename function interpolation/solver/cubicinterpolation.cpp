#include "cubicinterpolation.h"
#include <cmath>


// Вычисляем производные только во внутренних узлах
// Используются m[2] ... m[n-3]
void cubic_interpolation(int n, double* x, double* f, double*& m)
{
    m = new double[n];

    for (int i = 2; i <= n - 3; ++i)
    {
        m[i] = (f[i + 1] - f[i - 1]) / (x[i + 1] - x[i - 1]);
    }
}

static double first_segment(
    double x0, double x1, double x2,
    double f0, double f1, double f2,
    double d2,
    double point)
{
    double A[4][5] = {
        {pow(x0,3), pow(x0,2), x0, 1, f0},
        {pow(x1,3), pow(x1,2), x1, 1, f1},
        {pow(x2,3), pow(x2,2), x2, 1, f2},
        {3*pow(x2,2), 2*x2, 1, 0, d2}
    };

    int col_perm[4] = {0,1,2,3};

    for (int k = 0; k < 4; ++k)
    {
        // Поиск главного элемента по всей подматрице
        double max_val = 0.0;
        int pivot_row = k;
        int pivot_col = k;

        for (int i = k; i < 4; ++i)
        {
            for (int j = k; j < 4; ++j)
            {
                if (fabs(A[i][j]) > max_val)
                {
                    max_val = fabs(A[i][j]);
                    pivot_row = i;
                    pivot_col = j;
                }
            }
        }

        // Перестановка строк
        if (pivot_row != k)
        {
            for (int j = 0; j < 5; ++j)
                std::swap(A[k][j], A[pivot_row][j]);
        }

        // Перестановка столбцов
        if (pivot_col != k)
        {
            for (int i = 0; i < 4; ++i)
                std::swap(A[i][k], A[i][pivot_col]);

            std::swap(col_perm[k], col_perm[pivot_col]);
        }

        // Делим строку на главный элемент
        double pivot = A[k][k];
        for (int j = k; j < 5; ++j)
            A[k][j] /= pivot;

        // Обнуление остальных строк
        for (int i = 0; i < 4; ++i)
        {
            if (i == k) continue;

            double factor = A[i][k];
            for (int j = k; j < 5; ++j)
                A[i][j] -= factor * A[k][j];
        }
    }

    // Решение с учетом перестановки столбцов
    double temp_solution[4];
    for (int i = 0; i < 4; ++i)
        temp_solution[i] = A[i][4];

    double solution[4];
    for (int i = 0; i < 4; ++i)
        solution[col_perm[i]] = temp_solution[i];

    double a = solution[0];
    double b = solution[1];
    double c = solution[2];
    double d = solution[3];

    return a*pow(point,3) + b*pow(point,2) + c*point + d;
}

// Стандартный эрмитов кусок
static double hermite_segment(
    double x0, double x1,
    double f0, double f1,
    double m0, double m1,
    double point)
{
    double h = x1 - x0;
    double t = (point - x0) / h;

    double h00 =  2*t*t*t - 3*t*t + 1;
    double h01 = -2*t*t*t + 3*t*t;
    double h10 =  t*t*t - 2*t*t + t;
    double h11 =  t*t*t - t*t;

    return f0*h00 + f1*h01 + h*m0*h10 + h*m1*h11;
}

// Вычисление значения интерполяции
double evaluate_cubic(int n, double* x, double* f, double* m, double point)
{
    // Первый объединённый участок [x0, x2]
    if (point <= x[2])
    {
        return first_segment(
            x[0], x[1], x[2],
            f[0], f[1], f[2],
            m[2],
            point
        );
    }

    // Последний объединённый участок [x[n-3], x[n-1]]
    if (point >= x[n - 3])
    {
        return first_segment(
            x[n - 1], x[n - 2], x[n - 3],
            f[n - 1], f[n - 2], f[n - 3],
            m[n - 3],
            point
        );
    }

    // Внутренние эрмитовы участки
    for (int i = 2; i <= n - 4; ++i)
    {
        if (point >= x[i] && point <= x[i + 1])
        {
            return hermite_segment(
                x[i], x[i + 1],
                f[i], f[i + 1],
                m[i], m[i + 1],
                point
            );
        }
    }

    return 0;
}