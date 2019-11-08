#ifndef EXPERIMENTWINDOW_H
#define EXPERIMENTWINDOW_H

#include <QMainWindow>
#include "mainwindow.h"
#include <QtCharts>
#include <chartwindow.h>

namespace Ui {
class ExperimentWindow;
}

class ExperimentWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ExperimentWindow(QWidget *parent = nullptr);
    ~ExperimentWindow();

private slots:
    void on_pushButton_back_clicked();

    void on_pushButton_open_clicked();

    void on_pushButton_experiment_clicked();

    QVector<QtCharts::QChartView*> experiment();

private:
    Ui::ExperimentWindow *ui;
    QWidget *parent;
    ChartWindow* fw;
    ChartWindow* ow;
    std::string pattern;
    int err, m;
    unsigned long int n;
    std::string text;
    QString fName;
};

#endif // EXPERIMENTWINDOW_H
