#ifndef CAPACITOR_H
#define CAPACITOR_H
#include "linearcomponent.h"
class CircuitSimulation;
class Capacitor : public LinearComponent
{
protected:
    double capacitance;
    static QSet<unsigned> index_list;
public:
    Capacitor(unsigned int INDEX);
    QRectF boundingRect() const;
    void paint(QPainter *painter,const QStyleOptionGraphicsItem *option,QWidget *widget);
    void rotate();
    void moveTo(const QPointF scenePoint);
    ~Capacitor();
    friend class CircuitSimulation;
};

#endif // CAPACITOR_H
