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

public:
    bool initCheck{false};

private:
    Ui::MainWindow *ui_;
    CaptivePortal *cp_;
    QStringList intfnamelist_;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    bool setIntfnameList();

private slots:
    void on_btnStartStop_clicked();
};
