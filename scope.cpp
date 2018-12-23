#include "scope.h"
#include "ui_scope.h"
Scope::Scope(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Scope)
{
    ui->setupUi(this);
    for(int i=0;i<11;i++){
        labelVoltage[i]=new QLabel("aaa",ui->centralwidget);
        labelVoltage[i]->setGeometry(5,405-i*40,50,15);
        labelVoltage[i]->setAlignment(Qt::AlignRight);
    }
    for(int i=0;i<11;i++){
        labelCurrent[i]=new QLabel("aaa",ui->centralwidget);
        labelCurrent[i]->setGeometry(1240,405-i*40,50,15);
        labelCurrent[i]->setAlignment(Qt::AlignLeft);
    }
    for(int i=0;i<11;i++){
        labelTime[i]=new QLabel("aaa",ui->centralwidget);
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
