#include "chartwindow.h"
#include "ui_chartwindow.h"

using namespace QtCharts;

ChartWindow::ChartWindow(QWidget *parent, QChartView *chart) :
    QMainWindow(parent),
    ui(new Ui::ChartWindow)
{
    ui->setupUi(this);
    this->setCentralWidget(chart);
    this->resize(800, 600);
   
}

ChartWindow::~ChartWindow()
{
    delete ui;
}
