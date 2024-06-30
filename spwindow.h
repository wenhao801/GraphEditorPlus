#ifndef SPWINDOW_H
#define SPWINDOW_H

#include <QWidget>
#include <QGraphicsItem>
#include <QList>
#include <QTableWidget>

class MyScene;
class MyNode;
class MyEdge;
class SPWindow : public QWidget
{
    Q_OBJECT
public:
    explicit SPWindow(MyScene *scene, QWidget *parent = nullptr);
    MyScene *scene;
    QTableWidget tableWidget;
    QList <MyNode*> V;
    QList <MyEdge*> E;
    QMap <QPair<MyNode*, MyNode*>, MyEdge*> from, first;
    QMap <QPair<MyNode*, MyNode*>, QString> ans;

    bool updateSP(QList <QGraphicsItem*> items);
protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void selectCell(int r, int c);
};

#endif // SPWINDOW_H
