#ifndef INSERTFROMTEXT_H
#define INSERTFROMTEXT_H

#include <QDialog>

namespace Ui {
class InsertFromText;
}

class MyScene;

class InsertFromText : public QDialog
{
    Q_OBJECT

public:
    explicit InsertFromText(QWidget *parent = nullptr);
    ~InsertFromText();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::InsertFromText *ui;
    MyScene* scene;
};

#endif // INSERTFROMTEXT_H
