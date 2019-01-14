#ifndef INDUCTOR_H
#define INDUCTOR_H
#include "linearcomponent.h"
#include <complex>
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
    std::complex <double> getimpedance(double frequency);
    ~Inductor();
    friend class  CircuitSimulation;
    friend QDataStream& operator<<( QDataStream&, const Inductor& );
    friend QDataStream& operator>>( QDataStream&, Inductor& );
};

#endif // INDUCTOR_H
