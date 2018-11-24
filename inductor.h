#ifndef INDUCTOR_H
#define INDUCTOR_H
#include "linearcomponent.h"
class CircuitSimulation;
class Inductor : public LinearComponent
{
protected:
    double inductance;
public:
    Inductor();
    friend class  CircuitSimulation;
};

#endif // INDUCTOR_H
