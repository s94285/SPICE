#ifndef GROUND_H
#define GROUND_H
#include "basiccomponent.h"

class ground : public BasicComponent
{
    QPoint port1;
public:
    ground();
    friend class CircuitSimulation;
};

#endif // GROUND_H
