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
    QPoint port1;
    QPoint port2;
    double current;
public:
    Source(unsigned int INDEX);
    QRectF boundingRect() const;
    void paint(QPainter *painter,const QStyleOptionGraphicsItem *option,QWidget *widget);
    friend class CircuitSimulation;
    ~Source();
    void rotate();
    void moveTo(const QPointF scenePoint);
};

#endif // SOURCE_H
