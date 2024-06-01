#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "insertfromtext.h"
#include "mydata.h"

#include <qicon.h>
#include <QToolButton>
#include <QFileDialog>
#include <QMessageBox>


int flag_isOpen = 0;
int flag_isNew = 0;
QString Last_FileName;

using Qt::endl;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->toolBar->addWidget(ui->moveMode); ui->modesGroup->setId(ui->moveMode, 0);
    ui->toolBar->addWidget(ui->selectMode); ui->modesGroup->setId(ui->selectMode, 1);
    ui->toolBar->addWidget(ui->addMode); ui->modesGroup->setId(ui->addMode, 2);
    ui->toolBar->addWidget(ui->deleteMode); ui->modesGroup->setId(ui->deleteMode, 3);
    ui->toolBar->addSeparator();
    ui->toolBar->addWidget(ui->toggleDirect);

    scene = new MyScene(this, ui->graphicsView, ui->nodeCount, ui->edgeCount, ui->spStatus);

    connect(ui->modesGroup, &QButtonGroup::buttonClicked, this, &MainWindow::switchMode);
    connect(ui->toggleDirect, &QToolButton::clicked, scene, &MyScene::toggleDirect);

    ui->graphicsView->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    ui->graphicsView->setScene(scene);

    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setMouseTracking(1);
    ui->graphicsView->viewport()->setMouseTracking(1);


    scene->setBackgroundBrush(Qt::white);

    statusBar()->addPermanentWidget(ui->nodeCount, 1);
    statusBar()->addPermanentWidget(ui->edgeCount, 1);
    statusBar()->addPermanentWidget(ui->spStatus, 1);

    flag_isNew = flag_isOpen = 1;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::switchMode() {
    scene->switchMode(MyScene::CursorMode(ui->modesGroup->checkedId()));
}


void MainWindow::on_actionFrom_Text_triggered()
{
    // qDebug() << "Inserting from text" << Qt::endl;
    InsertFromText window(this);
    window.exec();
}

void MainWindow::on_actionNew_triggered()
{
    scene->clear();
    scene->nodes.clear();
    scene->edges.clear();
    scene->ids.clear();
    scene->defaultNodeID = 1;
    flag_isNew = 1;
    flag_isOpen = 1;
}

void MainWindow::on_actionSave_triggered()
{
    if(flag_isNew)
    {
        QFileDialog fileDialog;
        QString str = fileDialog.getSaveFileName(this,"Open File","","Text File(*.txt)");
        if(str == "")
        {
            return;
        }
        QFile filename(str);
        if(!filename.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QMessageBox::warning(this,"error","Open File Error!");
            return;
        }
        else
        {
            QTextStream textStream(&filename);

            textStream << "Nodes" << endl;
            for(auto item: scene->nodes){
                textStream << item->pos().x() << ' ' << item->pos().y() << ' ' << item->name->text() << endl;
            }
            textStream << "Edges" << endl;
            for(auto item: scene->edges){
                textStream << item->startNode->name->text() << ' ' << item->endNode->name->text() << ' ' << item->weight->text() << endl;
            }
            textStream << "defaultNodeID" << ' ' << scene->defaultNodeID << endl;

        }
        QMessageBox::information(this,"Ssve File","Save File Success",QMessageBox::Ok);
        filename.close();
        flag_isNew = 0;
        Last_FileName = str;
    }
    else
    {
        if(flag_isOpen)
        {
            QFile file(Last_FileName);
            if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
            {
                QMessageBox::warning(this,"error","Open File Failed");
                return;
            }
            else
            {
                QTextStream textStream(&file);
                textStream << "Nodes" << endl;
                for(auto item: scene->nodes){
                    qDebug() << item->pos().x() << ' ' << item->pos().y() << ' ' << item->name->text() << endl;
                    textStream << item->pos().x() << ' ' << item->pos().y() << ' ' << item->name->text() << endl;
                }
                textStream << "Edges" << endl;
                for(auto item: scene->edges){
                    qDebug() << item->startNode->name->text() << ' ' << item->endNode->name->text() << ' ' << item->weight->text() << endl;
                    textStream << item->startNode->name->text() << ' ' << item->endNode->name->text() << ' ' << item->weight->text() << endl;
                }
                textStream << "defaultNodeID" << ' ' << scene->defaultNodeID << endl;
                file.close();
            }
        }
        else
        {
            QMessageBox::warning(this,"Warning","Please new or open a file");
            return;
        }
    }
}

void MainWindow::on_actionOpen_triggered()
{
    QString fileName;

    fileName = QFileDialog::getOpenFileName(this,"Open File","","Text File(*.txt)");

    if(fileName == "")
    {
        return ;
    }
    else
    {
        QFile file(fileName);
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QMessageBox::warning(this,"error","open file error!");
            return;
        }
        else
        {
            if(!file.isReadable())
                QMessageBox::warning(this,"error","this file is not readable!");
            else
            {
                QTextStream textStream(&file);
                QStringList readList = textStream.readAll().split('\n', Qt::SkipEmptyParts);

                scene->clear();
                scene->ids.clear();
                scene->edges.clear();
                scene->nodes.clear();

                bool edgeTurn = 0;
                for(auto line: readList){
                    QStringList lineList = line.split(' ');
                    if(lineList.size() == 1){
                        if(lineList[0] == "Edges") edgeTurn = 1;
                        continue;
                    }
                    else{
                        if (lineList[0] == "defaultNodeID") {
                            scene->defaultNodeID = lineList[1].toInt();
                            continue;
                        }
                        if(edgeTurn){

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
                file.close();
                flag_isOpen = 1;
                Last_FileName = fileName;
            }
        }
    }
}

void MainWindow::on_actionSaveAs_triggered(){
    QFileDialog fileDialog;
    QString fileName = fileDialog.getSaveFileName(this,"Open File","","Text File(*.txt)");
    if(fileName == "")
    {
        return;
    }
    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::warning(this,"error","open file failure!");
        return;
    }
    else
    {
        QTextStream textStream(&file);

        textStream << "Nodes" << endl;
        for(auto item: scene->nodes){
            textStream << item->pos().x() << ' ' << item->pos().y() << ' ' << item->name->text() << endl;
        }
        textStream << "Edges" << endl;
        for(auto item: scene->edges){
            textStream << item->startNode->name->text() << ' ' << item->endNode->name->text() << ' ' << item->weight->text() << endl;
        }
        textStream << "defaultNodeID" << ' ' << scene->defaultNodeID << endl;

        QMessageBox::warning(this,"tip","Save File Success!");
        Last_FileName = fileName;
        flag_isNew = 0;
        file.close();
    }
}

void MainWindow::on_actionSet_autoindex_triggered()
{
    int mex = 1; while (scene->ids.count(QString::number(mex))) ++mex;
    bool ok;
    int res = QInputDialog::getInt(this, "Setting starting point for auto-index",
                                 "Setting starting point for auto-index to...(default: the smallest available positive number)",
                                    mex, -2147483647, 2147483647, 1, &ok);
    if (ok) scene->defaultNodeID = res;
}

