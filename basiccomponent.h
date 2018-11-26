#ifndef BASICCOMPONENT_H
#define BASICCOMPONENT_H
#include <QMainWindow>
class CircuitSimulation;
class BasicComponent
{
protected:
    unsigned index;
    QString name;
public:
    BasicComponent();
    friend class CircuitSimulation;
};

#endif // BASICCOMPONENT_H
