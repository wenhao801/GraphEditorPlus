#include "insertfromtext.h"
#include "ui_insertfromtext.h"

#include "mainwindow.h"

InsertFromText::InsertFromText(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::InsertFromText)
{
    ui->setupUi(this);
    scene = ((MainWindow*)parent)->scene;
}

InsertFromText::~InsertFromText()
{
    delete ui;
}

void InsertFromText::on_buttonBox_accepted()
{
    QString text = ui->plainTextEdit->document()->toPlainText();
    scene->insertFromText(text);
}

