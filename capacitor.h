#ifndef CAPACITOR_H
#define CAPACITOR_H
#include "linearcomponent.h"
class CircuitSimulation;
class Capacitor : public LinearComponent
{
protected:
    double capacitance;
public:
    Capacitor();
    friend class CircuitSimulation;
};

#endif // CAPACITOR_H
