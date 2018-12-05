#ifndef RESISITOR_H
#define RESISITOR_H
#include "linearcomponent.h"
class CircuitSimulation;
class Resisitor : public LinearComponent
{
protected:
    double resistance;
public:
    Resisitor(unsigned int INDEX);
    QRectF boundingRect() const;
    void paint(QPainter *painter,const QStyleOptionGraphicsItem *option,QWidget *widget);
    void rotate();
    void moveTo(const QPointF scenePoint);
    ~Resisitor();
    friend class CircuitSimulation;
};

#endif // RESISITOR_H
