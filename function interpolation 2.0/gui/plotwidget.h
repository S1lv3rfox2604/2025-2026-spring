#ifndef PLOTWIDGET_H
#define PLOTWIDGET_H

#include <QWidget>

class MainWindow;

class PlotWidget : public QWidget
{
    Q_OBJECT 
public:
    PlotWidget(MainWindow* mainWin, QWidget* parent = nullptr);
    virtual ~PlotWidget() = default;  // Добавить виртуальный деструктор

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    MainWindow* m_mainWin;
};

#endif