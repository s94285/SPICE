#include "source.h"
Source::Source(unsigned int INDEX)
{
    index=INDEX;
    name="V"+QString::number(index);
    value="0";
    pixmap = new QPixmap(":/resources/source.png");
    width=pixmap->width();
    height=pixmap->height();
    bound=new QRectF(-width/2.,-height/2.,width,height);
    port1=QPoint(anker_x,anker_y);
}
Source::~Source(){
    delete pixmap;
    delete bound;
}

QRectF Source::boundingRect() const{
    return *bound;
}
void Source::paint(QPainter *painter,const QStyleOptionGraphicsItem *option,QWidget *widget){
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->drawPixmap(-width/2,-height/2,*pixmap);
    painter->drawText(21,-46,name);
    painter->drawText(21,50,value);
}
void Source::rotate(){
    this->setRotation(this->rotation()+90);
}
void Source::moveTo(const QPointF scenePoint){
    anker_x=round(scenePoint.x())/pixPerAnker;
    anker_y=round(scenePoint.y())/pixPerAnker;
    setPos(anker_x*pixPerAnker,anker_y*pixPerAnker);
}
