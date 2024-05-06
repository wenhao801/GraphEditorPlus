#include "mydialog1.h"
#include "ui_mydialog1.h"

MyDialog1::MyDialog1(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::MyDialog1)
{
    ui->setupUi(this);

    connect(ui->pushButton, &QPushButton::clicked, this, [](){qDebug() << "button clicked"; });
}

MyDialog1::~MyDialog1()
{
    delete ui;
}

// void MyDialog1::sendSignalToMain() {} itself is a signal, like QPushButton::clicked

void MyDialog1::on_pushButton_clicked()
{
    ui->pushButton->setText("here clicked");
    emit sendSignalToMain();
}
