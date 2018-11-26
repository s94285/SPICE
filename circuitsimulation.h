#ifndef CIRCUITSIMULATION_H
#define CIRCUITSIMULATION_H
#include "node.h"
#include "resisitor.h"
#include "source.h"
#include "inductor.h"
#include "capacitor.h"
#include "mainwindow.h"
#include <QVector>
class CircuitSimulation : public QObject
{
    Q_OBJECT
private:
    MainWindow *mainWindow;
    QVector<BasicComponent*> components;
signals:
private slots:
    void run();
    void drawLine();
    void addGround();
    void addResistor();
    void addCapacitor();
    void addInductor();
    void cut();
    void move();
public:
    CircuitSimulation();
    ~CircuitSimulation();
    void init();
};

#endif // CIRCUITSIMULATION_H
