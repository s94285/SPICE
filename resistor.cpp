#include "resistor.h"

Resistor::Resistor(unsigned int INDEX)
{
    index=INDEX;
    name="R"+QString::number(index);
    value="0";
    pixmap = new QPixmap(":/resources/resistor.png");
    width=pixmap->width();
    height=pixmap->height();
    bound=new QRectF(-width/2.,-height/2.,width,height);
}
Resistor::~Resistor(){
    delete pixmap;
    delete bound;
}
QRectF Resistor::boundingRect() const{
    return *bound;
}
void Resistor::paint(QPainter *painter,const QStyleOptionGraphicsItem *option,QWidget *widget){
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->drawPixmap(-width/2,-height/2,*pixmap);
    painter->drawText(23,-10,name);
    painter->drawText(25,20,value);
}
void Resistor::rotate(){
    this->setRotation(this->rotation()+90);
}
void Resistor::moveTo(const QPointF scenePoint){
    anker_x=round(scenePoint.x())/pixPerAnker;
    anker_y=round(scenePoint.y())/pixPerAnker;
    setPos(anker_x*pixPerAnker,anker_y*pixPerAnker);
}
