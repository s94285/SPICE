#include "scope.h"
#include "ui_scope.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
Scope::Scope(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Scope)
{
    ui->setupUi(this);
    for(int i=0;i<11;i++){
        labelVoltage[i]=new QLabel("",ui->centralwidget);
        labelVoltage[i]->setGeometry(5,405-i*40,50,15);
        labelVoltage[i]->setAlignment(Qt::AlignRight);
    }
    for(int i=0;i<11;i++){
        labelCurrent[i]=new QLabel("",ui->centralwidget);
        labelCurrent[i]->setGeometry(1240,405-i*40,50,15);
        labelCurrent[i]->setAlignment(Qt::AlignLeft);
    }
    for(int i=0;i<11;i++){
        labelTime[i]=new QLabel("",ui->centralwidget);
        labelTime[i]->setGeometry(55+i*117,425,50,15);
        labelTime[i]->setAlignment(Qt::AlignLeft);
    }
    ui->scopeView->setScope(this);
    ui->scopeView->init();
}

Scope::~Scope()
{
    delete ui;
}

void Scope::closeEvent(QCloseEvent *event)
{
    MainWindow *mainWindow = dynamic_cast<MainWindow*>(this->parent());
    if(mainWindow){
        mainWindow->ui->workspace->currentMode=IDLE;
    }
    QMainWindow::closeEvent(event);
}
