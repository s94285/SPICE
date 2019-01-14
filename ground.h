#ifndef GROUND_H
#define GROUND_H
#include "basiccomponent.h"
class CircuitSimulation;
class ground : public BasicComponent
{
    static QSet<unsigned> index_list;
public:
    ground(unsigned int INDEX);
    QRectF boundingRect() const;
    void paint(QPainter *painter,const QStyleOptionGraphicsItem *option,QWidget *widget);
    void rotate();
    void moveTo(const QPointF scenePoint);
    ~ground();
    friend class CircuitSimulation;
    friend QDataStream& operator<<( QDataStream&, const ground& );
    friend QDataStream& operator>>( QDataStream&, ground& );
};

#endif // GROUND_H
