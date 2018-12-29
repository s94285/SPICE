#include "basiccomponent.h"
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
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

QPoint *BasicComponent::getPortByScenePoint(const QPointF &itemPoint)
{
    for(QPoint &pt:ports){
       // qDebug() << pt << endl;
        if(round(itemPoint.x()/pixPerAnker)==pt.x()&&round(itemPoint.y()/pixPerAnker)==pt.y()){
            return &pt;
        }
    }
    return nullptr;
}
const int BasicComponent::pixPerAnker = 15;
//QSet<unsigned> BasicComponent::index_list;
