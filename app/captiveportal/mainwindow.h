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
    Ui::MainWindow *ui;
    CaptivePortal *cp;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnStartStop_clicked();
};
