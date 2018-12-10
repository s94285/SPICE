#ifndef LINE_H
#define LINE_H
#include <QGraphicsItem>
class CircuitSimulation;
class Line : public QGraphicsItem
{
protected:
    QPoint point1,point2;
    int pixPerAnker;
public:
    Line(const QPoint &pt1,const QPoint &pt2,QGraphicsItem * parent = nullptr);
    Line(const QPointF &pt1,const QPointF &pt2,QGraphicsItem * parent = nullptr);
    QRectF boundingRect() const;
    void paint(QPainter *painter,const QStyleOptionGraphicsItem *option,QWidget *widget);
    QPoint getPoint1(){return point1;}
    QPoint getPoint2(){return point2;}
    QPoint setPoint1(QPoint point){point1=point;}
    QPoint setPoint2(QPoint point){point2=point;}
    QPoint setPoint1(QPointF pointf);
    QPoint setPoint2(QPointF pointf);
    friend class CircuitSimulation;
};

#endif // LINE_H
