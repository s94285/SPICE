#ifndef SOURCE_H
#define SOURCE_H
#include "basiccomponent.h"
class CircuitSimulation;

class Source : public BasicComponent
{
protected:
    double amplitude;
    double freq;
    double phase;
public:
    Source();
    QRectF boundingRect() const;
    void paint(QPainter *painter,const QStyleOptionGraphicsItem *option,QWidget *widget);
    friend class CircuitSimulation;
};

#endif // SOURCE_H
