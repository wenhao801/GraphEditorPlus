#ifndef EDITWINDOW_H
#define EDITWINDOW_H

#include <QWidget>

#include <QHBoxLayout>
#include <QHeaderView>
#include <QCheckBox>
#include <QGraphicsItem>
#include <QList>
#include "consoletable.h"

class MyScene;
class EditWindow : public QWidget
{
    Q_OBJECT
public:
    explicit EditWindow(MyScene *scene, QWidget *parent = nullptr);
    ConsoleTable *tableWidget;
    QList <QGraphicsItem*> items;
    MyScene *scene;
    void updateTable();
    bool userEditing = 0;
protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void toggleSelection(QTableWidgetItem *item);
    void editItem(QTableWidgetItem *item);
};

#endif // EDITWINDOW_H
