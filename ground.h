#ifndef GROUND_H
#define GROUND_H
#include "basiccomponent.h"
class CircuitSimulation;
class ground : public BasicComponent
{
    QPoint port1;
    static QSet<unsigned> index_list;
public:
    ground(unsigned int INDEX);
    QRectF boundingRect() const;
    void paint(QPainter *painter,const QStyleOptionGraphicsItem *option,QWidget *widget);
    void rotate();
    void moveTo(const QPointF scenePoint);
    ~ground();
    friend class CircuitSimulation;
};

#endif // GROUND_H
