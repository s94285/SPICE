#include "circuitsimulation.h"
#include <QDebug>
#include <ui_mainwindow.h>
CircuitSimulation::CircuitSimulation()
{
    mainWindow=new MainWindow;

    //connecting buttons
    connect((mainWindow->toolBarButton)[0],SIGNAL(clicked()),this,SLOT(run()));
    connect((mainWindow->toolBarButton)[1],SIGNAL(clicked()),this,SLOT(drawLine()));
    connect((mainWindow->toolBarButton)[2],SIGNAL(clicked()),this,SLOT(addGround()));
    connect((mainWindow->toolBarButton)[3],SIGNAL(clicked()),this,SLOT(addResistor()));
    connect((mainWindow->toolBarButton)[4],SIGNAL(clicked()),this,SLOT(addCapacitor()));
    connect((mainWindow->toolBarButton)[5],SIGNAL(clicked()),this,SLOT(addInductor()));
    connect((mainWindow->toolBarButton)[6],SIGNAL(clicked()),this,SLOT(cut()));
    connect((mainWindow->toolBarButton)[7],SIGNAL(clicked()),this,SLOT(move()));
    mainWindow->show();
}

CircuitSimulation::~CircuitSimulation(){
    for(auto i:components)
        delete i;   //clean up all components
    delete mainWindow;
}

void CircuitSimulation::init(){

    Source *s1=new Source;

    components.append(s1);
    mainWindow->ui->workspace->drawComponents(components);
}

void CircuitSimulation::run(){qDebug()<<"Run\n";}
void CircuitSimulation::drawLine(){qDebug()<<"drawLine\n";}
void CircuitSimulation::addGround(){qDebug()<<"addGround\n";}
void CircuitSimulation::addResistor(){qDebug()<<"addResistor\n";}
void CircuitSimulation::addCapacitor(){qDebug()<<"addCapacitor\n";}
void CircuitSimulation::addInductor(){qDebug()<<"addInductor\n";}
void CircuitSimulation::cut(){qDebug()<<"cut\n";}
void CircuitSimulation::move(){qDebug()<<"move\n";}
