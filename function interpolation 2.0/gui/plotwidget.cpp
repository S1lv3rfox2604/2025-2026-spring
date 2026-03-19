#include "plotwidget.h"
#include "mainwindow.h"
#include "../input/input.h"
#include "../solver/chebyshev.h"
#include "../solver/cubicinterpolation.h"
#include <QPainter>
#include <QDebug>
#include <cmath>

PlotWidget::PlotWidget(MainWindow* mainWin, QWidget* parent)
    : QWidget(parent), m_mainWin(mainWin)
{
    setFocusPolicy(Qt::StrongFocus);
}

void PlotWidget::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    int w = width();
    int h = height();

    double a = m_mainWin->getA();
    double b = m_mainWin->getB();
    int n = m_mainWin->getN();
    int k = m_mainWin->getK();
    int p = m_mainWin->getP();
    int scale = m_mainWin->getScale();
    int mode = m_mainWin->getMode();

    double mid = (a + b) / 2;
    double half_len = (b - a) / 2;
    double cur_half = half_len / std::pow(2.0, scale);
    double x_left = mid - cur_half;
    double x_right = mid + cur_half;

    enum FuncType { Exact, Cheb, Cubic, ErrCheb, ErrCubic };

    std::vector<FuncType> to_draw;
    switch (mode) {
        case 0:
            to_draw = {Exact, Cheb};
            break;
        case 1:
            to_draw = {Exact};
            if (n >= 4) to_draw.push_back(Cubic);
            break;
        case 2:
            to_draw = {Exact, Cheb};
            if (n >= 4) to_draw.push_back(Cubic);
            break;
        case 3:
            to_draw = {ErrCheb};
            if (n >= 4) to_draw.push_back(ErrCubic);
            break;
    }

    const int NPOINTS = 1000;
    double min_y = INFINITY, max_y = -INFINITY;
    double max_abs = 0.0;

    auto eval = [&](FuncType type, double x) -> double {
        switch (type) {
            case Exact:
                return analytical_function(x, k);
            case Cheb:
                {
                    auto& xc = m_mainWin->getXCheb();
                    auto& coeffs = m_mainWin->getCoeffsCheb();
                    return evaluate_chebyshev(n, xc.data(), coeffs.data(), x);
                }
            case Cubic:
                {
                    if (n < 4) return 0.0;
                    auto& xu = m_mainWin->getXUniform();
                    auto& fu = m_mainWin->getFUniform();
                    auto& m = m_mainWin->getMCubic();
                    return evaluate_cubic(n, xu.data(), fu.data(), m.data(), x);
                }
            case ErrCheb:
                {
                    double exact = analytical_function(x, k);
                    auto& xc = m_mainWin->getXCheb();
                    auto& coeffs = m_mainWin->getCoeffsCheb();
                    double cheb = evaluate_chebyshev(n, xc.data(), coeffs.data(), x);
                    return exact - cheb;
                }
            case ErrCubic:
                {
                    double exact = analytical_function(x, k);
                    if (n < 4) return 0.0;
                    auto& xu = m_mainWin->getXUniform();
                    auto& fu = m_mainWin->getFUniform();
                    auto& m = m_mainWin->getMCubic();
                    double cubic = evaluate_cubic(n, xu.data(), fu.data(), m.data(), x);
                    return exact - cubic;
                }
        }
        return 0.0;
    };

    for (int i = 0; i <= NPOINTS; ++i) {
        double t = double(i) / NPOINTS;
        double x = x_left + t * (x_right - x_left);
        for (auto type : to_draw) {
            double y = eval(type, x);
            if (std::isfinite(y)) {
                if (y < min_y) min_y = y;
                if (y > max_y) max_y = y;
                if (std::fabs(y) > max_abs) max_abs = std::fabs(y);
            }
        }
    }

    if (min_y > max_y) {
        min_y = -1.0;
        max_y = 1.0;
    }

    qDebug() << "max|F| = " << max_abs;

    auto toX = [&](double x) -> int {
        return int((x - x_left) / (x_right - x_left) * w);
    };
    auto toY = [&](double y) -> int {
        return h - int((y - min_y) / (max_y - min_y) * h);
    };

    painter.setPen(Qt::black);
    int y0 = toY(0.0);
    if (y0 >= 0 && y0 < h) painter.drawLine(0, y0, w, y0);
    int x0 = toX(0.0);
    if (x0 >= 0 && x0 < w) painter.drawLine(x0, 0, x0, h);

    auto penForType = [](FuncType type) -> QPen {
        switch (type) {
            case Exact: return QPen(Qt::blue);
            case Cheb:  return QPen(Qt::red);
            case Cubic: return QPen(Qt::green);
            case ErrCheb: return QPen(Qt::magenta);
            case ErrCubic: return QPen(Qt::cyan);
        }
        return QPen(Qt::black);
    };

    for (auto type : to_draw) {
        QPolygonF poly;
        for (int i = 0; i <= NPOINTS; ++i) {
            double t = double(i) / NPOINTS;
            double x = x_left + t * (x_right - x_left);
            double y = eval(type, x);
            if (std::isfinite(y)) {
                poly << QPointF(toX(x), toY(y));
            } else {
                if (!poly.isEmpty()) {
                    painter.setPen(penForType(type));
                    painter.drawPolyline(poly);
                    poly.clear();
                }
            }
        }
        if (!poly.isEmpty()) {
            painter.setPen(penForType(type));
            painter.drawPolyline(poly);
        }
    }

    QString funcStr;
    switch (k) {
        case 1: funcStr = "f(x)=1"; break;
        case 2: funcStr = "f(x)=x"; break;
        case 3: funcStr = "f(x)=x^2"; break;
        case 4: funcStr = "f(x)=x^3"; break;
        case 5: funcStr = "f(x)=x^4"; break;
        case 6: funcStr = "f(x)=exp(x)"; break;
        case 7: funcStr = "f(x)=1/(1+25x^2)"; break;
    }
    QString info = QString("k=%1 %2  scale=%3  n=%4  p=%5  max|F|=%6")
                   .arg(k).arg(funcStr).arg(scale).arg(n).arg(p).arg(max_abs);
    painter.setPen(Qt::black);
    painter.drawText(10, 20, info);
}