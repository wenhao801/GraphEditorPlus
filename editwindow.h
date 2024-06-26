#ifndef EDITWINDOW_H
#define EDITWINDOW_H

#include <QWidget>

#include <QTableWidget>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QCheckBox>
#include <QGraphicsItem>
#include <QList>

class MyScene;
class EditWindow : public QWidget
{
    Q_OBJECT
public:
    explicit EditWindow(MyScene *scene, QWidget *parent = nullptr);
    QTableWidget *tableWidget;
    QList <QGraphicsItem*> items;
    MyScene *scene;
    void updateTable();
protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void onItemClicked(QTableWidgetItem *item);
};

#endif // EDITWINDOW_H
