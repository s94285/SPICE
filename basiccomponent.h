#ifndef BASICCOMPONENT_H
#define BASICCOMPONENT_H
#include <QMainWindow>
#include <QGraphicsItem>
#include <QPainter>
class CircuitSimulation;
class BasicComponent : public QGraphicsItem
{
protected:
    unsigned index;
    QString name;
    QString value;
    //uses for QGraphicsItem
    int width,height;
    int anker_x,anker_y;
    QPixmap *pixmap;
    QRectF *bound;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    bool _pan;
    int _panStartX, _panStartY;
public:
    BasicComponent();
    //pure virtual functions uses for QGraphicsItem, must be re defined
    virtual QRectF boundingRect() const = 0;
    virtual void paint(QPainter *painter,const QStyleOptionGraphicsItem *option,QWidget *widget) = 0;
    virtual void rotate()=0;
    virtual void moveTo(const QPointF scenePoint)=0;
    friend class CircuitSimulation;
    const static int pixPerAnker;
};

#endif // BASICCOMPONENT_H
