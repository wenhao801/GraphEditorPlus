/********************************************************************************
** Form generated from reading UI file 'mydialog1.ui'
**
** Created by: Qt User Interface Compiler version 6.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MYDIALOG1_H
#define UI_MYDIALOG1_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_MyDialog1
{
public:
    QPushButton *pushButton;

    void setupUi(QDialog *MyDialog1)
    {
        if (MyDialog1->objectName().isEmpty())
            MyDialog1->setObjectName("MyDialog1");
        MyDialog1->resize(400, 300);
        pushButton = new QPushButton(MyDialog1);
        pushButton->setObjectName("pushButton");
        pushButton->setGeometry(QRect(130, 140, 75, 23));

        retranslateUi(MyDialog1);

        QMetaObject::connectSlotsByName(MyDialog1);
    } // setupUi

    void retranslateUi(QDialog *MyDialog1)
    {
        MyDialog1->setWindowTitle(QCoreApplication::translate("MyDialog1", "Dialog", nullptr));
        pushButton->setText(QCoreApplication::translate("MyDialog1", "PushButton", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MyDialog1: public Ui_MyDialog1 {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MYDIALOG1_H
