#include "mainwindow.h"
#include "plotwidget.h"
#include "../input/input.h"
#include "../solver/chebyshev.h"
#include "../solver/cubicinterpolation.h"
#include <QKeyEvent>
#include <cmath>

MainWindow::MainWindow(double a, double b, int n, int k, int method, QWidget* parent)
    : QMainWindow(parent), a_(a), b_(b), n_(n), k_(k), p_(0), scale_(0)
{
    if (method == 1) mode_ = 0;
    else if (method == 2) mode_ = 1;
    else mode_ = 2;

    plotWidget_ = new PlotWidget(this, this);
    setCentralWidget(plotWidget_);
    setFocusPolicy(Qt::StrongFocus);
    updateData(true, true);
}

void MainWindow::recomputeUniform()
{
    if (n_ < 4) {
        x_uniform_.clear();
        f_uniform_.clear();
        m_cubic_.clear();
        return;
    }

    x_uniform_.resize(n_);
    f_uniform_.resize(n_);

    for (int i = 0; i < n_; ++i) {
        x_uniform_[i] = a_ + i * (b_ - a_) / (n_ - 1);
        f_uniform_[i] = analytical_function(x_uniform_[i], k_);
    }

    max_f_abs_ = 0.0;
    for (int i = 0; i < n_; ++i) {
        double abs_val = std::fabs(f_uniform_[i]);
        if (abs_val > max_f_abs_) max_f_abs_ = abs_val;
    }

    int idx = n_ / 2;
    f_uniform_[idx] += p_ * 0.1 * max_f_abs_;

    m_cubic_.resize(n_);
    cubic_interpolation(n_, x_uniform_.data(), f_uniform_.data(), m_cubic_.data());
}

void MainWindow::recomputeCheb()
{
    if (n_ < 1) return;
    x_cheb_.resize(n_);
    coeffs_cheb_.resize(n_);
    chebyshev_interpolation(a_, b_, n_, k_, x_cheb_.data(), coeffs_cheb_.data());
}

void MainWindow::updateData(bool updateCheb, bool updateCubic)
{
    if (updateCheb) recomputeCheb();
    if (updateCubic) recomputeUniform();
}

void MainWindow::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_0) {
        k_ = k_ % 7 + 1;
        updateData(true, true);
        plotWidget_->update();
    }
    else if (event->key() == Qt::Key_1) {
        mode_ = (mode_ + 1) % 4;
        plotWidget_->update();
    }
    else if (event->key() == Qt::Key_2) {
        scale_++;
        plotWidget_->update();
    }
    else if (event->key() == Qt::Key_3) {
        if (scale_ > 0) scale_--;
        plotWidget_->update();
    }
    else if (event->key() == Qt::Key_4) {
        if (n_ < 50) n_ *= 2;
        updateData(true, true);
        plotWidget_->update();
    }
    else if (event->key() == Qt::Key_5) {
        if (n_ > 2) n_ /= 2;
        updateData(true, true);
        plotWidget_->update();
    }
    else if (event->key() == Qt::Key_6) {
        p_++;
        updateData(false, true);
        plotWidget_->update();
    }
    else if (event->key() == Qt::Key_7) {
        p_--;
        updateData(false, true);
        plotWidget_->update();
    }
    else {
        QMainWindow::keyPressEvent(event);
    }
}