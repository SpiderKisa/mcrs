#ifndef GENERATIONWINDOW_H
#define GENERATIONWINDOW_H

#define MAX_COUNT 1000000
#define MAX_PATTERN_LENGTH 8
#define MIN_PATTERN_LENGTH 3


#include <QMainWindow>
#include "mainwindow.h"

namespace Ui {
class GenerationWindow;
}

enum TYPE_OF_ERR {INSERTION, DELETION, SUBSTITUTION};

class GenerationWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit GenerationWindow(QWidget *parent = nullptr);
    ~GenerationWindow();

private slots:
    void on_pushButton_back_clicked();

    void on_pushButton_generate_clicked();

    void generate();

    int randInt(int low, int high);


private:
    Ui::GenerationWindow *ui;
    QWidget *parent;
    QString pattern;
    int pattern_length;
    int err_min;
    int err_max;
    int count;
    bool correct = true;

};

#endif // GENERATIONWINDOW_H
