#ifndef LINEARCOMPONENT_H
#define LINEARCOMPONENT_H
#include "basiccomponent.h"
#include <QPoint>
class CircuitSimulation;
class LinearComponent : public BasicComponent
{
protected:
    QPoint port1;
    QPoint port2;
    double current;
public:
    LinearComponent();
    friend class CircuitSimulation;
};

#endif // LINEARCOMPONENT_H
