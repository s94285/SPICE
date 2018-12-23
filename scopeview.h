#ifndef SCOPEVIEW_H
#define SCOPEVIEW_H
#include <QGraphicsView>
#include "linearcomponent.h"
#include "node.h"
class ScopeView : public QGraphicsView
{
public:
    explicit ScopeView(QWidget *parent = nullptr);
    void init();
    void setRunTime(double RunTime){runTime=RunTime;}
    QVector<Node*> nodes;
    QVector<LinearComponent*> linearComponents;
    void clear();
    void draw();
private:
    QGraphicsScene *scene;
    QPixmap *pixmap1,*pixmap2;
    QPainter *painter1,*painter2;
    QGraphicsPixmapItem *item;
    QPointF *pointf;
    double runTime;
};

#endif // SCOPEVIEW_H
