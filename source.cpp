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
    bound=new QRectF(-width/2.,-height/2.-5,width,height+10);

    ports.append(QPoint(anker_x,anker_y-4));
    ports.append(QPoint(anker_x,anker_y+4));
}
Source::~Source(){
    index_list.remove(index);
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
    ports[0]=QPoint(-(ports[0].y()-anker_y)+anker_x,(ports[0].x()-anker_x)+anker_y); //rotate 90 degrees
    ports[1]=QPoint(-(ports[1].y()-anker_y)+anker_x,(ports[1].x()-anker_x)+anker_y);
    qDebug() << ports[0] << " , " << ports[1] << endl;
}
void Source::moveTo(const QPointF scenePoint){
    QPoint oldAnker=QPoint(anker_x,anker_y);
    anker_x=round(scenePoint.x()/pixPerAnker);
    anker_y=round(scenePoint.y()/pixPerAnker);
    setPos(anker_x*pixPerAnker,anker_y*pixPerAnker);
    ports[0]+=getAnkerPoint()-oldAnker;
    ports[1]+=getAnkerPoint()-oldAnker;
}

void Source::getdata(QString &getamp, QString &getfreq, QString &getphase, QString &getDCValue, QString &getVon, QString &getVinitial){
    double temp;
    double temparr[6]={amplitude,freq,phase,DCvalue,Von,Vinitial};
    qDebug() << temparr[0] << endl;
    QString *tempgetarr[6]={&getamp,&getfreq,&getphase,&getDCValue,&getVon,&getVinitial};
    for(int i=0;i<6;i++)
    {
        temp=temparr[i];
//        if((int)temp==0)
//            *(tempgetarr[i])=" ";
//        else
            *(tempgetarr[i])=QString::number(temparr[i]);

    }
}

void Source::setdata(QString getamp, QString getfreq, QString getphase, QString getDCValue, QString getVon, QString getVinitial){
    double *temparr[6]={&amplitude,&freq,&phase,&DCvalue,&Von,&Vinitial};
    QString tempgetarr[6]={getamp,getfreq,getphase,getDCValue,getVon,getVinitial};
    for(int i=0;i<6;i++)
    {
        *(temparr[i])=tempgetarr[i].toDouble();
        //*(&amplitude)=tempgetarr[0].toDouble();

    }
}
