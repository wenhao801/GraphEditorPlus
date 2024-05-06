#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "mydialog1.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_showSub_clicked()
{
    MyDialog1 *sub = new MyDialog1(this);
    connect(sub, &MyDialog1::sendSignalToMain, this, &MainWindow::subClicked);
    sub->setModal(1);
    sub->show();
}

void MainWindow::subClicked()
{
    qDebug() << "main called";
}
