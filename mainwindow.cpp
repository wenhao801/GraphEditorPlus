#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mydialog1.h"
#include <qicon.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    scene = new MyScene(this, ui->graphicsView);
    ui->graphicsView->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    ui->graphicsView->setScene(scene);

    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);

    ui->toolBar->addWidget(ui->toolButton_1);
    ui->toolBar->addWidget(ui->toolButton_2);
    ui->toolBar->addWidget(ui->toolButton_3);
    ui->toolBar->addWidget(ui->toolButton_4);

    scene->setBackgroundBrush(Qt::white);

    MyNode* u = scene->addNode(0, 0);

    MyNode* v = scene->addNode(80, 80);

    scene->addEdge(u, v);

    MyNode *w = scene->addNode(-120, 40);
    scene->addEdge(u, w);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_showSub_clicked()
{
    MyDialog1 *sub = new MyDialog1(this);
    connect(sub, &MyDialog1::sendSignalToMain, this, &MainWindow::subClicked);
    sub->setModal(1);
    sub->show();
}

void MainWindow::subClicked()
{
    qDebug() << "main called";
}

void MainWindow::on_addNode_clicked()
{
    qreal rx = QRandomGenerator::global()->generateDouble() * 200 - 100;
    qreal ry = QRandomGenerator::global()->generateDouble() * 200 - 100;
    scene->addNode(rx, ry);
}

