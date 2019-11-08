#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
//#include "generationwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_generation_clicked();

    void on_pushButton_experiment_clicked();

private:
    Ui::MainWindow *ui;
    //GenerationWindow *gWindow = nullptr;
};
#endif // MAINWINDOW_H
