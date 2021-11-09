#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(this->geometry().width(),this->geometry().height());
    ui->comboBox->addItems();
    initCheck = true;
}

MainWindow::~MainWindow()
{
    if(!(cp->close()))
    {
        qDebug() << "failed to close cp";
    }
    delete ui;
}

void MainWindow::on_btnStartStop_clicked()
{
    if(ui->btnStartStop->text() == "start")
    {
        qInfo() << "redirect url=" << ui->lineEdit->text()
                << ","
                << "intfname=" << ui->comboBox->currentText();
        cp = new CaptivePortal(ui->lineEdit->text(), ui->comboBox->currentText());
        ui->btnStartStop->setText("stop");
    }
    else if(ui->btnStartStop->text() == "stop")
    {
        if(!(cp->close()))
        {
            qDebug() << "failed to close cp";
            exit(1);
        }
        ui->btnStartStop->setText("start");
    }
}
