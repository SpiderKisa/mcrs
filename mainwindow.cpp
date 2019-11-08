#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "generationwindow.h"
#include "experimentwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_generation_clicked()
{
    GenerationWindow *gWindow = new GenerationWindow(this);
    gWindow->show();
    hide();
}

void MainWindow::on_pushButton_experiment_clicked()
{
    ExperimentWindow *eWindow = new ExperimentWindow(this);
    eWindow->show();
    hide();
}
