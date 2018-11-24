#ifndef SOURCE_H
#define SOURCE_H
class CircuitSimulation;

class Source
{
protected:
    double amplitude;
    double freq;
    unsigned int index;
    double phase;
public:
    Source();
    friend class CircuitSimulation;
};

#endif // SOURCE_H
