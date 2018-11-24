#ifndef BASICCOMPONENT_H
#define BASICCOMPONENT_H

class CircuitSimulation;
class BasicComponent
{
protected:
    unsigned index;
public:
    BasicComponent();
    friend class CircuitSimulation;
};

#endif // BASICCOMPONENT_H
