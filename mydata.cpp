#include "mydata.h"
#include "mynode.h"
#include "myedge.h"
#include <QDir>
#include <QMessageBox>

using Qt::endl;



MyData::MyData(MyScene* scene):nodes(&scene->nodes), edges(&scene->edges), undefinedItem(0){
    currentDirectory = QDir::currentPath();
    fileName = currentDirectory + tr("/undefined%d.txt").arg(undefinedItem);
}

bool MyData::saveToFile(const QString &filename){
    ++undefinedItem;
    QFile file(filename);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << "Nodes" << endl;
        for(auto item: *nodes){
            out << item->pos().x() << ' ' << item->pos().y() << ' ' << item->name->text() << endl;
        }
        out << "Edges" << endl;
        for(auto item: *edges){
            out << item->startNode->name->text() << ' ' << item->endNode->name->text() << ' ' << item->weight->text() << endl;
        }
        flag_isNew = 0;
        Last_FileName = filename;
        file.close();
        return 1;
    } else {
        qDebug() << "Could not open file for writing";
        return 0;
    }
    return 0;
}


void MyData::loadFromFile(const QString &filename, MyScene* scene){
    scene->clear();
    QFile file(filename);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        QStringList readList = in.readAll().split('\n', Qt::SkipEmptyParts);
        bool Edgessss = 0;
        for(auto line: readList){
            QStringList lineList = line.split(' ');
            if(lineList.size() == 1){
                if(lineList[0] == "Edges") Edgessss = 1;
                continue;
            }
            else{
                if(Edgessss){
                    if(lineList.size() == 2){
                        scene->addEdge(scene->ids[lineList[0]], scene->ids[lineList[1]]);
                    }
                    else if(lineList.size() == 3){
                        scene->addEdge(scene->ids[lineList[0]], scene->ids[lineList[1]], lineList[2]);
                    }
                    else{
                        qDebug() << "????????????Why" << endl;
                        break;
                    }
                }
                else{
                    if(lineList.size() == 3){
                        scene->addNode(lineList[0].toDouble(), lineList[1].toDouble(), lineList[2]);
                    }
                    else{
                        qDebug() << "???????? !!!!Why" << endl;
                        break;
                    }
                }
            }
        }
        flag_isOpen = 1;
        Last_FileName = filename;
        file.close();
    }
    else
    {
        qDebug() << "?!" << endl;
    }
}
