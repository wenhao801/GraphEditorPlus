#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QRandomGenerator>
#include <QStatusBar>

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

    MyScene *scene;

private slots:
    void on_addNode_clicked();

    void on_actionFrom_Text_triggered();

private:
    Ui::MainWindow *ui;

};
#endif // MAINWINDOW_H
