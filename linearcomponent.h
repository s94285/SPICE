#ifndef LINEARCOMPONENT_H
#define LINEARCOMPONENT_H
#include "basiccomponent.h"
#include <QPoint>
#include <complex>

class CircuitSimulation;
class ScopeView;
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
    friend class ScopeView;
    friend QDataStream &operator<<(QDataStream&,const LinearComponent&);
    friend QDataStream &operator>>(QDataStream&,LinearComponent&);
};

#endif // LINEARCOMPONENT_H
