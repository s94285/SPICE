#ifndef RESISITOR_H
#define RESISITOR_H
#include "linearcomponent.h"
class CircuitSimulation;
class Resistor : public LinearComponent
{
protected:
    double resistance;
    static QSet<unsigned> index_list;
public:
    Resistor(unsigned int INDEX);
    QRectF boundingRect() const;
    void paint(QPainter *painter,const QStyleOptionGraphicsItem *option,QWidget *widget);
    void rotate();
    void moveTo(const QPointF scenePoint);
    ~Resistor();
    friend class CircuitSimulation;
};

#endif // RESISITOR_H
