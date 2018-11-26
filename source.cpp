#include "source.h"

Source::Source()
{
    pixmap = new QPixmap(":/resources/source.png");
    width=pixmap->width();
    height=pixmap->height();
    bound=new QRectF(-width/2.,-height/2.,width,height);
}
QRectF Source::boundingRect() const{
    return *bound;
}
void Source::paint(QPainter *painter,const QStyleOptionGraphicsItem *option,QWidget *widget){
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->drawPixmap(-width/2,-height/2,*pixmap);
}
