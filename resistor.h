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
    void set(QString name,QString value);
    std::complex<double> getimpedance(double frequency);
    ~Resistor();
    friend class CircuitSimulation;
    friend QDataStream& operator<<( QDataStream&, const Resistor& );
    friend QDataStream& operator>>( QDataStream&, Resistor& );
};

#endif // RESISITOR_H
