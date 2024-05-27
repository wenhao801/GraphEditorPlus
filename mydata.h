#ifndef MYDATA_H
#define MYDATA_H

#include <set>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QObject>

#include "myscene.h"

class MyNode;
class MyEdge;
class MainWindow;

class MyData:public QObjectData
{
    Q_OBJECT
public:
    MyData(MyScene* scene);
    bool saveToFile(const QString &filename);
    void loadFromFile(const QString &filename, MyScene* scene);
private:
    std::set <MyNode*> *nodes;
    std::set <MyEdge*> *edges;
    QString fileName;
    QString currentDirectory;
    friend class MainWindow;
    int undefinedItem;

    int flag_isOpen = 0;
    int flag_isNew = 0;
    QString Last_FileName;
    QString Last_FileContent;
};

#endif // MYDATA_H
