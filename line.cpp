#include "line.h"
#include "QPainter"
#include "basiccomponent.h"
#include <QDebug>
Line::Line(const QPoint &pt1, const QPoint &pt2, QGraphicsItem *parent):QGraphicsItem (parent)
{
    pixPerAnker=BasicComponent::pixPerAnker;
    point1=pt1;
    point2=pt2;
}

Line::Line(const QPointF &pt1, const QPointF &pt2, QGraphicsItem *parent):QGraphicsItem (parent)
{
    pixPerAnker=BasicComponent::pixPerAnker;
    point1=QPoint(round(pt1.x()/pixPerAnker),round(pt1.y()/pixPerAnker));
    point2=QPoint(round(pt2.x()/pixPerAnker),round(pt2.y()/pixPerAnker));
    setPos(point1*pixPerAnker);
}

QRectF Line::boundingRect() const
{
    QRectF bound;
    bound.setTopLeft(QPointF(-8,-8));
    bound.setWidth((point2.x()-point1.x())*pixPerAnker+16);
    bound.setHeight((point2.y()-point1.y())*pixPerAnker+16);
    return bound;
}

void Line::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    setPos(point1*pixPerAnker);

//    qDebug() << "Paint\n";
    Q_UNUSED(option);
    Q_UNUSED(widget);
    QPen pen(Qt::black);

    pen.setWidth(2);
    painter->setPen(pen);

    painter->drawLine(QPointF(0,0),QPointF(pixPerAnker*(point2-point1)));
    pen.setColor(Qt::blue);
    painter->setPen(pen);
    if(point1Rect)
        painter->drawRect(-5,-5,10,10);
    if(point2Rect)
        painter->drawRect(QRectF(QPointF(pixPerAnker*(point2-point1))-QPointF(5,5),QSizeF(10,10)));
}

QPoint Line::setPoint1(QPointF pointf)
{
    point1.setX(round(pointf.x()/pixPerAnker));
    point1.setY(round(pointf.y()/pixPerAnker));
}

QPoint Line::setPoint2(QPointF pointf)
{
    point2.setX(round(pointf.x()/pixPerAnker));
    point2.setY(round(pointf.y()/pixPerAnker));
}
