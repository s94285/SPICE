#ifndef LINEARCOMPONENT_H
#define LINEARCOMPONENT_H
#include "basiccomponent.h"
#include <QPoint>
#include <complex>

class CircuitSimulation;
class LinearComponent : public BasicComponent
{
protected:
    QPoint port1;
    QPoint port2;
    double current;

public:
    LinearComponent();
    virtual std::complex<double> getimpedance(double frequency)=0;
    friend class CircuitSimulation;

};

#endif // LINEARCOMPONENT_H
