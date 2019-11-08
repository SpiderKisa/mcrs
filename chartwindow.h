#ifndef CHARTWINDOW_H
#define CHARTWINDOW_H

#include <QMainWindow>
#include <QtCharts>

namespace Ui {
class ChartWindow;
}

class ChartWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ChartWindow(QWidget *parent = nullptr,  QChartView *chart = nullptr);
    ~ChartWindow();

private:
    Ui::ChartWindow *ui;
};

#endif // CHARTWINDOW_H
