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

    void switchMode();

    MyScene *scene;

private slots:

    void on_actionFrom_Text_triggered();

    void on_actionNew_triggered();
    void on_actionSave_triggered();
    void on_actionOpen_triggered();
    void on_actionSaveAs_triggered();

    void on_actionSet_autoindex_triggered();

    void on_actionAuto_layout_triggered();

    void on_actionSelection_editor_triggered();

    void on_actionShortest_Path_triggered();

    void on_actionMinimum_spanning_tree_triggered();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
