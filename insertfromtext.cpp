#include "insertfromtext.h"
#include "ui_insertfromtext.h"

InsertFromText::InsertFromText(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::InsertFromText)
{
    ui->setupUi(this);
}

InsertFromText::~InsertFromText()
{
    delete ui;
}

void InsertFromText::on_buttonBox_accepted()
{
    QString text = ui->plainTextEdit->document()->toPlainText();
    QStringList lines = text.split('\n', Qt::SkipEmptyParts);
    for (auto line: lines) {
        QStringList words = line.split(' ', Qt::SkipEmptyParts);

    }
}

