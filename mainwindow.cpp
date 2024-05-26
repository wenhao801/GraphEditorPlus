#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "insertfromtext.h"

#include <qicon.h>
#include <QToolButton>

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
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::switchMode() {
    scene->switchMode(MyScene::CursorMode(ui->modesGroup->checkedId()));
}

void MainWindow::on_addNode_clicked()
{
    qreal rx = QRandomGenerator::global()->generateDouble() * 200 - 100;
    qreal ry = QRandomGenerator::global()->generateDouble() * 200 - 100;
    scene->addNode(rx, ry);
}


void MainWindow::on_actionFrom_Text_triggered()
{
    // qDebug() << "Inserting from text" << Qt::endl;
    InsertFromText window(this);
    window.exec();
}

