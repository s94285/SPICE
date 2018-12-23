#include "scopeview.h"
#include <cmath>
#include <QGraphicsPixmapItem>

ScopeView::ScopeView(QWidget *parent):QGraphicsView (parent)
{

}
void ScopeView::init()
{
    scene=new QGraphicsScene();
    pixmap1=new QPixmap(780,540);
    pixmap2=new QPixmap(780,540);
    painter1=new QPainter(pixmap1);
    painter2=new QPainter(pixmap2);
    pointf=new QPointF[10000];
    this->setScene(scene);
    painter1->fillRect(0,0,780,540,Qt::white);
    item=scene->addPixmap(*pixmap1);
    this->show();
}

void ScopeView::clear()
{

}

void ScopeView::draw()
{

}
