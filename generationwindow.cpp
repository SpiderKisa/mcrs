#include "generationwindow.h"
#include "ui_generationwindow.h"
#include <QTime>
#include <QFile>
#include<QDir>
#include <QTextStream>
#include <QVector>



GenerationWindow::GenerationWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GenerationWindow)
{
    ui->setupUi(this);
    this->parent = parent;

    ui->spinBox_min->setRange(MIN_PATTERN_LENGTH - 2, MAX_PATTERN_LENGTH - 2);
    ui->spinBox_max->setRange(MIN_PATTERN_LENGTH - 1, MAX_PATTERN_LENGTH - 1);
}



GenerationWindow::~GenerationWindow()
{
    delete ui;
}



void GenerationWindow::on_pushButton_back_clicked()
{
    hide();
    parent->show();
}



void GenerationWindow::on_pushButton_generate_clicked()
{

    correct = true;
    ui->label_errMinError->setText("");
    ui->label_errMaxError->setText("");
    ui->label_countError->setText("");
    ui->label_patternError->setText("");
    ui->label_patternError_2->setText("");

    pattern = ui->lineEdit_pattern->text();
    pattern_length = pattern.length();
    if (pattern_length ==  0){
        ui->label_patternError->setText("<font color='red'>Введите слово</font>");
        correct = false;
    }
    for(int i = 0; i < pattern_length; i++) {
        if ((!(pattern[i] >= 0x41 && pattern[i] <= 0x5a)) && (!(pattern[i] >= 0x61 && pattern[i] <= 0x7a))){
            ui->label_patternError->setText("<font color='red'>Образец должен состоять только из латинских букв</font>");
            i = pattern_length;
            correct = false;
        }
    }

    if (pattern_length > MAX_PATTERN_LENGTH || pattern_length < MIN_PATTERN_LENGTH){
        if (ui->label_patternError->text() == ""){
            ui->label_patternError->setText("<font color='red'>Длина слова должна находиться в диапазоне [" + QString::number(MIN_PATTERN_LENGTH) + ";" + QString::number(MAX_PATTERN_LENGTH) + "]</font>");
        } else {
            ui->label_patternError_2->setText("<font color='red'>Длина слова должна находиться в диапазоне [" + QString::number(MIN_PATTERN_LENGTH) + ";" + QString::number(MAX_PATTERN_LENGTH) + "]</font>");
        }
        correct = false;
    }
    err_min = ui->spinBox_min->value();
    err_max = ui->spinBox_max->value();
    if (err_min >= pattern_length){
        ui->label_errMinError->setText("<font color='red'>Минимальное число ошибок должно быть меньше длины образца</font>");
        correct = false;
    }
    if (err_max >= pattern_length){
        ui->label_errMaxError->setText("<font color='red'>Максимальное число ошибок должно быть меньше длины образца</font>");
        correct = false;
    }
    if (err_min >= err_max){
        if (ui->label_errMaxError->text() == ""){
            ui->label_errMaxError->setText("<font color='red'>Максимальное число ошибок должно быть больше минимального</font>");
        } else {
            ui->label_errMaxError_2->setText("<font color='red'>Максимальное число ошибок должно быть больше минимального</font>");
        }
        correct = false;
    }

    count = ui->lineEdit_count->text().toInt();

    if (count < 1 || count > MAX_COUNT){
        ui->label_countError->setText("<font color='red'>Количество слов должно находиться в диапазоне  [1; " + QString::number(MAX_COUNT) + "]</font>");
        correct = false;
    }

    if(correct){

        generate();

    }

}


void GenerationWindow::generate(){

    int x = 1;
    QString fileName = pattern + " " + QString::number(err_min) + "-" + QString::number(err_max) +
                        " " + QString::number(count) + " (" + QString::number(x) + ").txt";
    QDir::setCurrent("C:/Users/super hacka/Desktop/course/build-bitap-Desktop_Qt_5_12_5_MinGW_32_bit-Debug/gen");
    while (QFile::exists(fileName)){
        x++;
        int bracket = fileName.indexOf('(');
        fileName.remove(bracket + 1, fileName.indexOf(')') - bracket - 1);
        fileName.insert(bracket + 1, QString::number(x));
    }

    QFile f(fileName);

    f.open(QFile::WriteOnly | QFile::Text);

    QTextStream in(&f);

    pattern = pattern.toLower();

    QTime time = QTime::currentTime();//stolen
    qsrand((uint)time.msec());//stolen

    for (int i = err_min; i <= err_max; i++){ //количество ошибок

        for (int j = 0; j < count; j++){//количество слов в одной строке (с опр кол-вом ошибок)

            QString word = pattern;
            int pos;
            QChar letter;
            QVector<QVector<bool>> positions(pattern_length);
            positions.fill({false, false, false});


            for (int k = 1; k <= i; k++){//делаем ошибки в образце

                TYPE_OF_ERR err = (TYPE_OF_ERR)randInt(0, 2);

                pos = randInt(0, word.length() - 1);

                letter = (QChar)randInt(0x61, 0x7a);

                switch (err) {
                    case INSERTION:
                        while ( positions[pos][2] == true){
                            pos = randInt(0, word.length() - 1);
                        }

                        word.insert(pos, letter);
                        positions.insert(pos,{true, false, false});

                    break;
                    case SUBSTITUTION:
                        while (positions[pos][0] != false){
                            pos = randInt(0, word.length() - 1);
                        }
                        while (word[pos] == letter){
                            letter = (QChar)randInt(0x61, 0x7a);
                        }
                        word[pos] = letter;
                        positions[pos][1] = true;
                    break;
                    case DELETION:
                        while (positions[pos][0] != false || positions[pos][1] != false){
                            pos = randInt(0, word.length() - 1);
                        }
                        positions.remove(pos, 1);
                        if (pos != positions.size()){
                            positions[pos][2] = true;
                        }
                        word.remove(pos, 1);
                    break;
                }
            }

            in << word << " ";          //записать слово в файл
        }

        in <<'\n';
    }

    f.flush();
    f.close();

}


int GenerationWindow::randInt(int low, int high){
    return qrand() % ((high + 1) - low) + low;
}
