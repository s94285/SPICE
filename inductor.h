#ifndef INDUCTOR_H
#define INDUCTOR_H
#include "linearcomponent.h"
class CircuitSimulation;
class Inductor : public LinearComponent
{
protected:
    double inductance;
    static QSet<unsigned> index_list;
public:
    Inductor(unsigned int INDEX);
    QRectF boundingRect() const;
    void paint(QPainter *painter,const QStyleOptionGraphicsItem *option,QWidget *widget);
    void rotate();
    void moveTo(const QPointF scenePoint);
    void set(QString name,QString value);
    ~Inductor();
    friend class  CircuitSimulation;
};

#endif // INDUCTOR_H
