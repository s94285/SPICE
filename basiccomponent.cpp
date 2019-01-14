#include "basiccomponent.h"
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include "QString"
BasicComponent::BasicComponent()
{
    anker_x=anker_y=0;
}

BasicComponent::~BasicComponent()
{
   // index_list.remove(index);
}

void BasicComponent::mouseMoveEvent(QGraphicsSceneMouseEvent *event){
    qDebug() << "ITEM MOVE: " << event->scenePos().x() << "," << event->scenePos().y() << endl;
//    anker_x=round(event->scenePos().x())/pixPerAnker;
//    anker_y=round(event->scenePos().y())/pixPerAnker;
//    setPos(anker_x*pixPerAnker,anker_y*pixPerAnker);
}
void BasicComponent::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){
    qDebug() << "ITEM RELEASE: " << event->scenePos().x() << "," << event->scenePos().y() << endl;
}
void BasicComponent::mousePressEvent(QGraphicsSceneMouseEvent *event){
    qDebug() << "ITEM PRESS: " << event->scenePos().x() << "," << event->scenePos().y() << endl;
}
void BasicComponent::set(QString cname,QString cvalue)
{

    name=cname;
    value=cvalue;


}

QString BasicComponent::alphabet(QString s)
{
    QString stemp;
    stemp=s;
    if(stemp[stemp.length()-1]=="k")
    {
        stemp.remove("k");
        double temp=stemp.toDouble();
        temp*=1000;
        stemp=QString::number(temp);
    }
    else if(stemp[stemp.length()-1]=="m")
    {
        stemp.remove("m");
        double temp=stemp.toDouble();
        temp*=0.001;
        stemp=QString::number(temp);
    }
    else if(stemp[stemp.length()-1]=="u")
    {
        stemp.remove("u");
        double temp=stemp.toDouble();
        temp*=0.000001;
        stemp=QString::number(temp);
    }
    else if(stemp[stemp.length()-1]=="n")
    {
        stemp.remove("n");
        double temp=stemp.toDouble();
        temp*=0.000000001;
        stemp=QString::number(temp);
    }
        return stemp;
}

QPoint *BasicComponent::getPortByScenePoint(const QPointF &itemPoint)
{
    for(QPoint &pt:ports){
//        qDebug() << pt << endl;
        if(round(itemPoint.x()/pixPerAnker)==pt.x()&&round(itemPoint.y()/pixPerAnker)==pt.y()){
            return &pt;
        }
    }
    return nullptr;
}

QDataStream &operator<<(QDataStream &qs, const BasicComponent &b)
{
    qs << b.pos() << b.rotation();
    qs << b.index << b.name << b.value << b.width << b.height << b.anker_x << b.anker_y << b.ports;
    return qs;
}

QDataStream &operator>>(QDataStream &qs, BasicComponent &b)
{
    QPointF pos;
    qreal rotation;
    qs >> pos >> rotation;
    b.setPos(pos);
    b.setRotation(rotation);
    qs >> b.index >> b.name >> b.value >> b.width >> b.height >> b.anker_x >> b.anker_y >> b.ports;
    return qs;
}
const int BasicComponent::pixPerAnker = 15;
//QSet<unsigned> BasicComponent::index_list;
