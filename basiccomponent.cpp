#include "basiccomponent.h"
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
BasicComponent::BasicComponent()
{
    anker_x=anker_y=0;
}

void BasicComponent::mouseMoveEvent(QGraphicsSceneMouseEvent *event){
    qDebug() << "ITEM MOVE: " << event->scenePos().x() << "," << event->scenePos().y() << endl;
    anker_x=round(event->scenePos().x())/pixPerAnker;
    anker_y=round(event->scenePos().y())/pixPerAnker;
    setPos(anker_x*pixPerAnker,anker_y*pixPerAnker);
}
void BasicComponent::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){
    qDebug() << "ITEM RELEASE: " << event->scenePos().x() << "," << event->scenePos().y() << endl;
}
void BasicComponent::mousePressEvent(QGraphicsSceneMouseEvent *event){
    qDebug() << "ITEM PRESS: " << event->scenePos().x() << "," << event->scenePos().y() << endl;
}

const int BasicComponent::pixPerAnker = 15;
