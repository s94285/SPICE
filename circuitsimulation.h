#ifndef CIRCUITSIMULATION_H
#define CIRCUITSIMULATION_H
#include "node.h"
#include "resistor.h"
#include "source.h"
#include "inductor.h"
#include "capacitor.h"
#include "ground.h"
#include "mainwindow.h"
#include "workspace.h"
#include "line.h"
#include <QVector>

class CircuitSimulation : public QObject
{
    Q_OBJECT
private:
    MainWindow *mainWindow;
    QVector<BasicComponent*> components;
    QVector<Node*> nodes;
    QVector<Line*> lines;
    Workspace *workspace;
private slots:
    void run();
    void drawLine();
    void addGround();
    void addResistor();
    void addCapacitor();
    void addInductor();
    void cut();
    void move();
    void addSource();
public:
    CircuitSimulation();
    ~CircuitSimulation();
    void init();
};

#endif // CIRCUITSIMULATION_H
