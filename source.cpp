#include "source.h"
#include <QDebug>
Source::Source(unsigned int INDEX)
{
    index=INDEX;
    name="V"+QString::number(index);
    value="0";
    pixmap = new QPixmap(":/resources/source.png");
    width=pixmap->width();
    height=pixmap->height();
    bound=new QRectF(-width/2.,-height/2.,width,height);

    port1=QPoint(anker_x,anker_y-4);
    port2=QPoint(anker_x,anker_y+4);
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
    qreal currentRotation = rotation();
    qDebug() << "Rotation = " << currentRotation << endl;
    painter->rotate(-currentRotation);
    if(fabs(rotation()-0)<1e-6){
        painter->drawText(21,-46,name);
        painter->drawText(21,50,value);
    }else if(fabs(rotation()-90)<1e-6){
        painter->drawText(-55,-21,name);
        painter->drawText(42,-21,value);
    }else if(fabs(rotation()-180)<1e-6){
        painter->drawText(21,-46,name);
        painter->drawText(21,50,value);
    }else if(fabs(rotation()-270)<1e-6){
        painter->drawText(-55,-21,name);
        painter->drawText(42,-21,value);
    }
    painter->rotate(currentRotation);
}
void Source::rotate(){
    this->setRotation(this->rotation()+90);
    if(fabs(rotation()-360)<1e-6)
        this->setRotation(0);
    port1=QPoint(-(port1.y()-anker_y)+anker_x,(port1.x()-anker_x)+anker_y); //rotate 90 degrees
    port2=QPoint(-(port2.y()-anker_y)+anker_x,(port2.x()-anker_x)+anker_y);
    qDebug() << port1 << " , " << port2 << endl;
}
void Source::moveTo(const QPointF scenePoint){
    QPoint oldAnker=QPoint(anker_x,anker_y);
    anker_x=round(scenePoint.x())/pixPerAnker;
    anker_y=round(scenePoint.y())/pixPerAnker;
    setPos(anker_x*pixPerAnker,anker_y*pixPerAnker);
    port1+=getAnkerPoint()-oldAnker;
    port2+=getAnkerPoint()-oldAnker;
}
