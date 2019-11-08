#include "experimentwindow.h"
#include "ui_experimentwindow.h"
#include <QFileDialog>
#include <QTextStream>


using namespace QtCharts;


ExperimentWindow::ExperimentWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ExperimentWindow)
{
    ui->setupUi(this);
    this->parent = parent;

}

ExperimentWindow::~ExperimentWindow()
{
    delete ui;
}

void ExperimentWindow::on_pushButton_back_clicked()
{
    hide();
    parent->show();
}


void ExperimentWindow::on_pushButton_open_clicked()
{
    fName = QFileDialog::getOpenFileName(this,
                                                    tr("Открыть файл"),
                                                    "C:/Users/super hacka/Desktop/course/build-bitap-Desktop_Qt_5_12_5_MinGW_32_bit-Debug/gen",
                                                    tr("Файл .txt (*.txt)"));


}

void ExperimentWindow::on_pushButton_experiment_clicked()
{
    bool correct = true;
    if (fName == ""){
        ui->label_errFile->setText("<font color='red'>Файл не выбран</font>");
        correct = false;
    }
    QString fileName = fName;
    fileName.remove(0, fileName.indexOf("gen/") + 4);
    fileName.remove(0, fileName.indexOf("-") + 1);
    err = fileName.left(fileName.indexOf(' ')).toInt();
    QFile f(fName);
     f.open(QFile::ReadOnly | QFile::Text);
     QTextStream out(&f);
     QString txt = out.readAll();
     text = txt.toLocal8Bit().constData();
     n = text.length();

    pattern = ui->lineEdit_pattern->text().toLocal8Bit().constData();
    int m = pattern.length();
    if (m ==  0){
        ui->label_err->setText("<font color='red'>Введите слово</font>");
        correct = false;
    }
    for(int i = 0; i < m; i++) {
        if ((!(pattern[i] >= 0x41 && pattern[i] <= 0x5a)) && (!(pattern[i] >= 0x61 && pattern[i] <= 0x7a))){
            ui->label_err->setText("<font color='red'>Образец должен состоять только из латинских букв</font>");
            i = m;
            correct = false;
        }
    }

    if (m > MAX_PATTERN_LENGTH || m < MIN_PATTERN_LENGTH){
        if (ui->label_err->text() == ""){
            ui->label_err->setText("<font color='red'>Длина слова должна находиться в диапазоне [" + QString::number(MIN_PATTERN_LENGTH) + ";" + QString::number(MAX_PATTERN_LENGTH) + "]</font>");
        } else {
            ui->label_err2->setText("<font color='red'>Длина слова должна находиться в диапазоне [" + QString::number(MIN_PATTERN_LENGTH) + ";" + QString::number(MAX_PATTERN_LENGTH) + "]в</font>");
        }
        correct = false;
    }



    if (correct){
        QVector<QChartView*> view = experiment();
        fw = new ChartWindow(this, view[0]);
        fw->show();

        ow = new ChartWindow(this, view[1]);
        ow->show();

    }
}

QVector<QChartView*> ExperimentWindow::experiment(){

    int found = 0;
    int op = 0;
     QStringList categories;
     QBarSeries *series_found = new QBarSeries();
    QBarSet *set_found = new QBarSet("Количество найденных слов");
    //QLineSeries *series_found = new QLineSeries();
    QLineSeries *series_op = new QLineSeries();


    int x = text.find(' ');
        while(x != std::string::npos){
            text.erase(x, 1);
            x = text.find(' ');
        }

        x = text.find('\n');
        while (x != text.length() - 1){
            text.erase(x, 1);
            x = text.find('\n');
        }

        std::string alph = "abcdefghijklmnopqrstuvwxyz";

        //std::string current_locale_text = qs.toLocal8Bit().constData();

       int alph_size = 26;

       int m = pattern.length();
       int n = text.length();

       std::vector<unsigned long> masks;

       for (int i = 0; i < alph_size; i++){
           masks.push_back(0);
       }

       for (int i = 0; i < alph_size; i++){
           int pos = 0;
           int j = 0;
           while (pos != std::string::npos){
               pos = pattern.find(alph[i], pos + j);
               unsigned long sh = 1;
               if (pos != std::string::npos){
                   sh = sh << (unsigned long)(m - pos - 1);
                   masks[i] = masks[i] | sh;
                   j++;
               }
           }
       }

       std::vector<std::vector<unsigned long>> r;
       std::vector<unsigned long> zero;
       for (int j = 0; j <= n; j++){
           zero.push_back(0);
       }
       for (int i = 0; i <= err; i++){
           r.push_back(zero);

       }

       unsigned long sh = 0;
       sh = 1 << (m - 1);
       for (int j = 1; j <= n; j++){
           r[0][j] = ((r[0][j-1] >> 1) | sh) & masks[text[j - 1] - 0x61];
           op += 3;
           if ((r[0][j] & (unsigned long)1) == 1){
                found++;
           }
       }
       categories << QString::number(0);
        *set_found << found;
       //series_found->append(0, found);
       series_op->append(0, op);


       for (int i = 1; i <= err; i++){
           sh = 1;
           sh = sh << (unsigned long)(m - 1);
           unsigned long bit = sh;
           for (int j = 1; j < i; j++){
               sh = (sh >> (unsigned long)1) | bit;
           }
           r[i][0] = sh;
       }

       sh = 1;
       sh = sh << (m - 1);

       for (int i = 1; i <= err; i++){

           found = 0;
           for (int j = 1; j <= n; j++){
               r[i][j] = (((r[i][j - 1] >> (unsigned long)1) | sh) & masks[text[j - 1] - 0x61]) |
                       (((r[i - 1][j - 1] | r[i - 1][j]) >> (unsigned long)1) | sh) | r[i - 1][j -1];
               op += 8;
               if ((r[i][j] & (unsigned long)1) == 1){

                   found++;
               }
           }
           categories << QString::number(i);
           *set_found << found;
           //series_found->append(i, found);
           series_op->append(i, op);

       }

       QVector<QChartView*> view;
        QChart *chart_found = new QChart();
        chart_found->legend()->hide();
        series_found->append(set_found);
        chart_found->addSeries(series_found);
        QBarCategoryAxis *axisX = new QBarCategoryAxis();
        axisX->append(categories);
        chart_found->addAxis(axisX, Qt::AlignBottom);
        series_found->attachAxis(axisX);
        QValueAxis *axisY = new QValueAxis();
        axisY->setRange(0, found);
        chart_found->addAxis(axisY, Qt::AlignLeft);
        series_found->attachAxis(axisY);
        chart_found->setTitle("Зависимость количества найденных слов от количества ошибок");
        QChartView *chartView_found = new QChartView(chart_found);
        chartView_found->setRenderHint(QPainter::Antialiasing);
        view.push_back(chartView_found);

        QChart *chart_op = new QChart();
        chart_op->legend()->hide();
        chart_op->addSeries(series_op);
        chart_op->createDefaultAxes();
        chart_op->setTitle("Зависимость количества битовых операций от количества ошибок");
        QChartView *chartView_op = new QChartView(chart_op);
        chartView_op->setRenderHint(QPainter::Antialiasing);
        view.push_back(chartView_op);

        return view;
}
