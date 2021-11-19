#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "weupdater.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui_(new Ui::MainWindow)
{
    ui_->setupUi(this);
    this->setFixedSize(this->geometry().width(),this->geometry().height());
    this->setWindowTitle("Captive Portal");
    if(setIntfnameList())
    {
        ui_->comboBox->addItems(intfnamelist_);
    }

    cp_ = new CaptivePortal(this);

    QJsonObject jo = GJson::loadFromFile();
    jo["MainWindow"] >> *cp_;

    ui_->lineEdit->setText(cp_->redirectpage_);
    ui_->comboBox->setCurrentText(cp_->intfname_);

    const char *v =
    #include "../../version.txt"
            ;

    WEUpdater weupdater;
    if(weupdater.checkVersion("http://wifievent.io/version/cp.txt", v))
    {
        QPalette redtext;
        redtext.setColor(QPalette::WindowText, Qt::red);
        ui_->label_5->setPalette(redtext);
        ui_->label_5->setText("There is new version");
        qDebug() << "something action to do in product(print notion that \'you have to update!\')\n";
    }
    else
    {
        QPalette blacktext;
        blacktext.setColor(QPalette::WindowText, Qt::black);
        ui_->label_5->setPalette(blacktext);
        ui_->label_5->setText("This is latest version");
        qDebug() << "there is no new version of product\n";
    }

    initCheck = true;
}

MainWindow::~MainWindow()
{
    if(!(cp_->close()))
    {
        qDebug() << "failed to close cp";
    }

    QJsonObject jo = GJson::loadFromFile();
    cp_->redirectpage_ = ui_->lineEdit->text();
    cp_->intfname_ = ui_->comboBox->currentText();
    jo["MainWindow"] << *cp_;
    GJson::saveToFile(jo);

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

GIp MainWindow::getGatewayIp()
{
    GIp gwIp = GNetInfo::instance().rtm().findGateway(
                ui_->comboBox->currentText(),
                GIp("8.8.8.8"));
    qInfo() << "gateway Ip:" << QString(gwIp);
    return gwIp;
}

void MainWindow::on_btnStartStop_clicked()
{
    if(ui_->btnStartStop->text() == "start")
    {
        qInfo() << "redirect url=" << ui_->lineEdit->text()
                << ","
                << "intfname=" << ui_->comboBox->currentText();
        cp_->intfname_ = ui_->comboBox->currentText();
        cp_->gwIp_ = getGatewayIp();
        cp_->redirectpage_ = ui_->lineEdit->text();
        cp_->open();
        ui_->btnStartStop->setText("stop");
    }
    else if(ui_->btnStartStop->text() == "stop")
    {
        if(!(cp_->close()))
        {
            qDebug() << "failed to close cp";
            exit(1);
        }
        ui_->btnStartStop->setText("start");
    }
}
