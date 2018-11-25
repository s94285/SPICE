#include "mainwindow.h"
#include <QApplication>
#include "circuitsimulation.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CircuitSimulation sim;
    sim.init();
    return a.exec();
}
