#include "workspace.h"
#include <QDebug>
#include <QMouseEvent>
#include <QGraphicsRectItem>
#include <QScrollBar>
#include "source.h"
Workspace::Workspace(QWidget *parent):QGraphicsView (parent)
{
    _pan=false;
    init();
}
void Workspace::init(){
//    this->setDragMode(QGraphicsView::ScrollHandDrag);
    scene = new QGraphicsScene();
    scene->setSceneRect(QRectF(-10000,-10000,20000,20000));
    this->setScene(scene);
//    scene->addItem(new QGraphicsRectItem(0,0,100,100));
    this->show();
}
void Workspace::drawComponents(QVector<BasicComponent*> &components_vector){
    scene->clear();
    for(BasicComponent* comp:components_vector){
        scene->addItem(comp);
    }
    this->show();
}
void Workspace::enterEvent(QEvent *event){
    QGraphicsView::enterEvent(event);
    viewport()->setCursor(Qt::CrossCursor);   //override default pan cursor
}
void Workspace::mousePressEvent(QMouseEvent *event){
    if(event->button()==Qt::MiddleButton){
        _pan=true;
        _panStartX=event->x();
        _panStartY=event->y();
        viewport()->setCursor(Qt::ClosedHandCursor);
        event->accept();
        return;
    }
    QGraphicsView::mousePressEvent(event);
    viewport()->setCursor(Qt::CrossCursor);   //override default pan cursor
}
void Workspace::mouseMoveEvent(QMouseEvent *event){
    if(_pan){
        horizontalScrollBar()->setValue(horizontalScrollBar()->value()-(event->x()-_panStartX));
        verticalScrollBar()->setValue(verticalScrollBar()->value()-(event->y()-_panStartY));
        _panStartX=event->x();
        _panStartY=event->y();
    }
//    qDebug() << event->x() << " , " << event->y() << endl;
    QGraphicsView::mouseMoveEvent(event);
    viewport()->setCursor(Qt::CrossCursor);   //override default pan cursor
}
void Workspace::mouseReleaseEvent(QMouseEvent *event){
    if(event->button()==Qt::MiddleButton){
        _pan=false;
        viewport()->setCursor(Qt::CrossCursor);
        event->accept();    //prevent for further propaganda
        return;
    }
    qDebug() << event->x() << " , " << event->y() << endl;
    QGraphicsView::mouseReleaseEvent(event);
    viewport()->setCursor(Qt::CrossCursor);   //override default pan cursor
}
