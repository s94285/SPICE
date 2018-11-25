#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //make toolbar buttons
    short int buttonIndex[8]={4,15,21,22,24,25,26,29};
    for(int i=0;i<8;i++){
        QPushButton *qb=new QPushButton;
        toolBarButton[i]=qb;
        qb->setFixedSize(30,30);
        qb->setIconSize(QSize(24,22));
        QPixmap qp(":/resources/button.png");
        QPixmap croped=qp.copy(QRect(buttonIndex[i]*24,0,24,22));
        qb->setIcon(croped);
        ui->mainToolBar->addWidget(qb);
    }
    //
}

MainWindow::~MainWindow()
{
    delete ui;
    for(auto tb:toolBarButton){
        delete tb;
    }
}
