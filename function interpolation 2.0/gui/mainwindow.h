#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vector>

class PlotWidget;
class QKeyEvent;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(double a, double b, int n, int k, int method, QWidget* parent = nullptr);
    virtual ~MainWindow() = default;

    double getA() const { return a_; }
    double getB() const { return b_; }
    int getN() const { return n_; }
    int getK() const { return k_; }
    int getP() const { return p_; }
    int getScale() const { return scale_; }
    int getMode() const { return mode_; }
    const std::vector<double>& getXUniform() const { return x_uniform_; }
    const std::vector<double>& getFUniform() const { return f_uniform_; }
    const std::vector<double>& getMCubic() const { return m_cubic_; }
    const std::vector<double>& getXCheb() const { return x_cheb_; }
    const std::vector<double>& getCoeffsCheb() const { return coeffs_cheb_; }
    double getMaxFAbs() const { return max_f_abs_; }

public slots:
    void updateData(bool updateCheb, bool updateCubic);

protected:
    void keyPressEvent(QKeyEvent* event) override;

private:
    double a_, b_;
    int n_, k_, p_;
    int scale_;
    int mode_;

    std::vector<double> x_uniform_, f_uniform_, m_cubic_;
    std::vector<double> x_cheb_, coeffs_cheb_;
    double max_f_abs_;

    PlotWidget* plotWidget_;

    void recomputeUniform();
    void recomputeCheb();
};

#endif