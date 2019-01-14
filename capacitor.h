#ifndef CAPACITOR_H
#define CAPACITOR_H
#include "linearcomponent.h"
#include <complex>
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
    void set(QString name,QString value);
    std::complex <double> getimpedance(double frequency);
    ~Capacitor();
    friend class CircuitSimulation;
    friend QDataStream& operator<<( QDataStream&, const Capacitor& );
    friend QDataStream& operator>>( QDataStream&, Capacitor& );
};

#endif // CAPACITOR_H
