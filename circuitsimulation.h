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
#include "scope.h"
#include <QMessageBox>
class CircuitSimulation : public QObject
{
    Q_OBJECT
    const static int scale=10000;
private:
    MainWindow *mainWindow;
    Scope *scopeWindow;
    QVector<BasicComponent*> components;
    QVector<Node*> nodes;
    QVector<Line*> lines;
    Workspace *workspace;
    QHash<void*,double*> points;
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
    void openFile();
    void saveFile();
    void newFile();
public:
    CircuitSimulation();
    ~CircuitSimulation();
    void init();
    void initRun();
    void drawFunction(Node*);
    void drawFunction(LinearComponent*);
    void drawFunction(Source*);
};

#endif // CIRCUITSIMULATION_H
