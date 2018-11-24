#ifndef NODE_H
#define NODE_H

class CircuitSimulation;
class Node
{
    double voltage;
    unsigned int index;
public:
    Node();
    friend class CircuitSimulation;
};

#endif // NODE_H
