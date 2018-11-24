#ifndef RESISITOR_H
#define RESISITOR_H
#include "linearcomponent.h"
class CircuitSimulation;
class Resisitor : public LinearComponent
{
protected:
    double resistance;
public:
    Resisitor();
    friend class CircuitSimulation;
};

#endif // RESISITOR_H
