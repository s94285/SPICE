#ifndef LINEARCOMPONENT_H
#define LINEARCOMPONENT_H
#include "basiccomponent.h"
#include <QPoint>
#include <complex>

class CircuitSimulation;
<<<<<<< HEAD
class ScopeView;
=======
class Scope;
>>>>>>> 06028826a411b787688781c9187e71f0479ca4e7
class LinearComponent : public BasicComponent
{
protected:
//    QPoint port1;
//    QPoint port2;
    QVector<QPair<std::complex<double>,double>> current;

public:
    LinearComponent();
    virtual std::complex<double> getimpedance(double frequency)=0;
    friend class CircuitSimulation;
<<<<<<< HEAD
    friend class ScopeView;
=======
    friend class Scope;
>>>>>>> 06028826a411b787688781c9187e71f0479ca4e7
};

#endif // LINEARCOMPONENT_H
