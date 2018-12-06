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
    workspace = mainWindow->ui->workspace;
}

CircuitSimulation::~CircuitSimulation(){
    for(auto i:components)
        delete i;   //clean up all components
    delete mainWindow;
}

void CircuitSimulation::init(){
    workspace->currentMode=IDLE;
    Source *s1=new Source(0);
    components.append(s1);
    workspace->setComponents(components);
    workspace->drawComponents();
}

void CircuitSimulation::run(){qDebug()<<"Run\n";}
void CircuitSimulation::drawLine(){qDebug()<<"drawLine\n";}
void CircuitSimulation::addGround(){qDebug()<<"addGround\n";}
void CircuitSimulation::addResistor(){
    qDebug()<<"addResistor\n";
    workspace->currentMode=IDLE;
    unsigned int ind=0;
    for(;;ind++)
        if(Resistor::index_list.find(ind)==Resistor::index_list.end())
            break;
    Resistor *r1=new Resistor(ind);
    Resistor::index_list.insert(ind);
    components.append(r1);
    workspace->drawComponents();
}
void CircuitSimulation::addCapacitor(){
    qDebug()<<"addCapacitor\n";
    workspace->currentMode=IDLE;
    unsigned int ind=0;
    for(;;ind++)
        if(Capacitor::index_list.find(ind)==Capacitor::index_list.end())
            break;
    Capacitor *c1=new Capacitor(ind);
    Capacitor::index_list.insert(ind);
    components.append(c1);
    workspace->drawComponents();
}
void CircuitSimulation::addInductor(){
    qDebug()<<"addInductor\n";
    workspace->currentMode=IDLE;
    unsigned int ind=0;
    for(;;ind++)
        if(Inductor::index_list.find(ind)==Inductor::index_list.end())
            break;
    Inductor *i1=new Inductor(ind);
    Inductor::index_list.insert(ind);
    components.append(i1);
    workspace->drawComponents();
}
void CircuitSimulation::cut(){
    qDebug()<<"cut\n";
    workspace->currentMode=CUT;
}
void CircuitSimulation::move(){
    qDebug()<<"move\n";
    workspace->currentMode=MOVE;
}
