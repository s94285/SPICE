#ifndef LINE_H
#define LINE_H
#include <QGraphicsItem>
class CircuitSimulation;
class Line : public QGraphicsItem
{
protected:
    QPoint point1,point2;
    int pixPerAnker;
    bool point1Rect=false,point2Rect=false;
public:
    Line(const QPoint &pt1,const QPoint &pt2,QGraphicsItem * parent = nullptr);
    Line(const QPointF &pt1,const QPointF &pt2,QGraphicsItem * parent = nullptr);
    QRectF boundingRect() const;
    void paint(QPainter *painter,const QStyleOptionGraphicsItem *option,QWidget *widget);
    QPoint getPoint1(){return point1;}
    QPoint getPoint2(){return point2;}
    QPoint setPoint1(QPoint point){point1=point;return point1;}
    QPoint setPoint2(QPoint point){point2=point;return point2;}
    QPoint setPoint1(QPointF pointf);
    QPoint setPoint2(QPointF pointf);
    bool getPoint1Rect(){return point1Rect;}
    bool getPoint2Rect(){return point2Rect;}
    bool setPoint1Rect(bool boolean){point1Rect=boolean;return point1Rect;}
    bool setPoint2Rect(bool boolean){point2Rect=boolean;return point2Rect;}
    friend class CircuitSimulation;
    friend QDataStream &operator<<(QDataStream&,const Line&);
    friend QDataStream &operator>>(QDataStream&,Line&);
};

#endif // LINE_H
