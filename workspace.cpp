#include "workspace.h"
#include <QDebug>
#include <QMouseEvent>
#include <QGraphicsRectItem>
#include <QScrollBar>
#include "source.h"
Workspace::Workspace(QWidget *parent):QGraphicsView (parent)
{
    _pan=false;
    itemSelected=nullptr;
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
    switch (currentMode) {
        case IDLE:
        viewport()->setCursor(Qt::CrossCursor);
        break;
        case MOVE:
        viewport()->setCursor(Qt::OpenHandCursor);
        break;
    }

}
void Workspace::mousePressEvent(QMouseEvent *event){
    switch(currentMode){
        case IDLE:
        if(event->button()==Qt::LeftButton){
            _pan=true;
            _panStartX=event->x();
            _panStartY=event->y();
            viewport()->setCursor(Qt::ClosedHandCursor);
            event->accept();
            return;
        }
        break;
        case MOVE:
        if(itemSelected){   //selected
            itemSelected=nullptr;
            currentMode=IDLE;
        }else{      //unselected
            itemSelected=dynamic_cast<BasicComponent*>(itemAt(event->pos()));
        }
        break;
    }

    QGraphicsView::mousePressEvent(event);
    viewport()->setCursor(Qt::CrossCursor);   //override default pan cursor
}
void Workspace::mouseMoveEvent(QMouseEvent *event){
    switch(currentMode){
        case IDLE:
        if(_pan){
            horizontalScrollBar()->setValue(horizontalScrollBar()->value()-(event->x()-_panStartX));
            verticalScrollBar()->setValue(verticalScrollBar()->value()-(event->y()-_panStartY));
            _panStartX=event->x();
            _panStartY=event->y();
        }
        break;
        case MOVE:
        if(itemSelected){
            itemSelected->moveTo(mapToScene(event->x(),event->y()));
        }
        break;
    }

    qDebug() << "viewMove " << event->x() << " , " << event->y() << endl;
    QGraphicsView::mouseMoveEvent(event);
}
void Workspace::mouseReleaseEvent(QMouseEvent *event){
    switch (currentMode) {
        case IDLE:
        if(event->button()==Qt::LeftButton){
            _pan=false;
            viewport()->setCursor(Qt::CrossCursor);
//            event->accept();    //prevent for further propaganda
//            return;
        }
        break;
    }

    qDebug() << "Scene " << mapToScene(event->pos()).x() << " , " << mapToScene(event->pos()).y() << endl;
    QGraphicsView::mouseReleaseEvent(event);
    viewport()->setCursor(Qt::CrossCursor);   //override default pan cursor
}

void Workspace::keyPressEvent(QKeyEvent *event){
    if(itemSelected){
        if(event->modifiers()==Qt::ControlModifier&&event->key()==Qt::Key_R){
            itemSelected->rotate();
        }
    }
}
