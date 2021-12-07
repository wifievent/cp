#pragma once

#include <QMainWindow>
#include "stdafx.h"
#include "captiveportal.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

    Ui::MainWindow *ui_;
    CaptivePortal *cp_;
    QStringList intfnamelist_;

public:
    bool initCheck{false};

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    bool setIntfnameList();
    Ip getGatewayIp();

private slots:
    void on_btnStartStop_clicked();
};
