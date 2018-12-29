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
    QVector<QPoint> ports;
public:
    BasicComponent();
    ~BasicComponent();
    //pure virtual functions uses for QGraphicsItem, must be re defined
    virtual QRectF boundingRect() const = 0;
    virtual void paint(QPainter *painter,const QStyleOptionGraphicsItem *option,QWidget *widget) = 0;
    virtual void rotate()=0;
    virtual void moveTo(const QPointF scenePoint)=0;
    void set(QString name,QString value);
    QString alphabet(QString s);
    const QString getName()const{return name;}
    const QString getValue()const{return value;}
    friend class CircuitSimulation;
    const static int pixPerAnker;
    const QPoint getAnkerPoint()const{return QPoint(anker_x,anker_y);}
    QPoint *getPortByScenePoint(const QPointF &itemPoint);
};

#endif // BASICCOMPONENT_H
