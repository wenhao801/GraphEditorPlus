#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mydialog1.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    scene = new MyScene(this);
    ui->graphicsView->setScene(scene);

    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);

    scene->setBackgroundBrush(Qt::white);

    MyNode* u = scene->addNode(0, 0);

    MyNode* v = scene->addNode(80, 80);

    MyEdge* e = new MyEdge(u, v);
    scene->addItem(e);
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

