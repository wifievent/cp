#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(this->geometry().width(),this->geometry().height());

    //this->setCentralWidget(ui->plainTextEdit);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnStartStop_clicked()
{
    if(ui->btnStartStop->text() == "start")
    {
        ui->btnStartStop->setText("stop");
    }
    else if(ui->btnStartStop->text() == "stop")
    {
        ui->btnStartStop->setText("start");
    }
}

void MainWindow::on_comboBox_activated(const QString &intfname)
{

}

