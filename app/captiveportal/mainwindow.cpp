#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui_(new Ui::MainWindow)
{
    ui_->setupUi(this);
    this->setFixedSize(this->geometry().width(),this->geometry().height());
    if(setIntfnameList())
    {
        ui_->comboBox->addItems(intfnamelist_);
    }
    initCheck = true;
}

MainWindow::~MainWindow()
{
    if(!(cp_->close()))
    {
        qDebug() << "failed to close cp";
    }
    if(cp_ != nullptr)
    {
        delete cp_;
    }
    delete ui_;
}

bool MainWindow::setIntfnameList()
{
    QString devname;
    pcap_if_t* dev;
    char errBuf[PCAP_ERRBUF_SIZE];
    int check = pcap_findalldevs(&dev, errBuf);
    if (check != 0)
    {
        qDebug() << "error";
        return false;
    }
    while (dev != nullptr)
    {
        devname = dev->name;
        qInfo() << "interface name: " << devname;
        intfnamelist_.append(devname);
        dev = dev->next;
    }
    pcap_freealldevs(dev);
    return true;
}

void MainWindow::on_btnStartStop_clicked()
{
    if(ui_->btnStartStop->text() == "start")
    {
        qInfo() << "redirect url=" << ui_->lineEdit->text()
                << ","
                << "intfname=" << ui_->comboBox->currentText();
        cp_ = new CaptivePortal(ui_->lineEdit->text(), ui_->comboBox->currentText());
        ui_->btnStartStop->setText("stop");
    }
    else if(ui_->btnStartStop->text() == "stop")
    {
        if(!(cp_->close()))
        {
            qDebug() << "failed to close cp";
            exit(1);
        }
        delete cp_;
        ui_->btnStartStop->setText("start");
    }
}
