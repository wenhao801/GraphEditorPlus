#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QRandomGenerator>

#include "myscene.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void subClicked();
    void switchMode();

private slots:
    void on_showSub_clicked();

    void on_addNode_clicked();

private:
    Ui::MainWindow *ui;

    MyScene *scene;
};
#endif // MAINWINDOW_H
