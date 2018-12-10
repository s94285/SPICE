#include "workspace.h"
#include <QDebug>
#include <QMouseEvent>
#include <QGraphicsRectItem>
#include <QScrollBar>
#include "source.h"
#include "resistor.h"
#include "inductor.h"
#include "capacitor.h"
#include "editdialog.h"
#include <QDialog>
#include "sourcedialog.h"
#include "line.h"
#include "node.h"
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

void Workspace::setComponents(QVector<BasicComponent *> &components_vector)
{
    components = &components_vector;
}
void Workspace::drawComponents(){
//    scene->clear();
    for(BasicComponent* comp:*components){
        scene->addItem(comp);
    }
    for(Node* no:*nodes){
        scene->addItem(no);
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
        case CUT:
        viewport()->setCursor(Qt::ForbiddenCursor);
        break;
        case DRAWLINE:
        viewport()->setCursor(Qt::BlankCursor);
        break;
    }

}
void Workspace::mousePressEvent(QMouseEvent *event){
    if(itemAt(event->pos()))qDebug() << "Clicked On Item";
    if(dynamic_cast<Node*>(itemAt(event->pos())))qDebug() << "Clicked on node";
    if(dynamic_cast<Line*>(itemAt(event->pos())))qDebug() << "Clicked on line with node: " << dynamic_cast<Node*>(dynamic_cast<Line*>(itemAt(event->pos()))->group());
    if(event->button()==Qt::RightButton&&(itemAt(event->pos())==nullptr)){
        currentMode=IDLE;
        viewport()->setCursor(Qt::CrossCursor);

        return;
    }
    if(event->button()==Qt::RightButton&&drawingline){
        //cancel drawingline
        Node *nodeOfLineToDelete=dynamic_cast<Node*>(drawingline->group());
        for(QGraphicsItem *line:nodeOfLineToDelete->childItems()){
            lines->removeOne(dynamic_cast<Line*>(line));
            nodeOfLineToDelete->removeFromGroup(line);
            delete line;
        }
        nodes->removeOne(nodeOfLineToDelete);
        delete nodeOfLineToDelete;
        drawingline=nullptr;
        scene->update();
    }
    if(event->button()==Qt::RightButton&&itemAt(event->pos())!=nullptr)
    {
        if(currentMode==DRAWLINE){
            currentMode=IDLE;
            viewport()->setCursor(Qt::CrossCursor);
            if(drawLineX){scene->removeItem(drawLineX);delete drawLineX;drawLineX=nullptr;}
            if(drawLineY){scene->removeItem(drawLineY);delete drawLineY;drawLineY=nullptr;}
            return;
        }
       /* Resistor *rptr;
        Inductor *iptr;
        Capacitor *cptr; */
        /*if(dynamic_cast<Resistor*>(itemAt(event->pos())))
        {
            dialog=new editDialog(this,dynamic_cast<Resistor*>(itemAt(event->pos())));
        }
        else if(dynamic_cast<Inductor*>(itemAt(event->pos())))
        {
            dialog=new editDialog(this,dynamic_cast<Inductor*>(itemAt(event->pos())));
        }
        else
        {
            dialog=new editDialog(this,dynamic_cast<Capacitor*>(itemAt(event->pos())));
        }*/
        if(dynamic_cast<Resistor*>(itemAt(event->pos()))!=nullptr||dynamic_cast<Inductor*>(itemAt(event->pos()))!=nullptr||dynamic_cast<Capacitor*>(itemAt(event->pos()))!=nullptr)
        {
            dialog=new editDialog(this,dynamic_cast<BasicComponent*>(itemAt(event->pos())));
            currentMode=IDLE;
            dialog->exec();
        }
        else if(dynamic_cast<Source*>(itemAt(event->pos()))!=nullptr)
        {
            sourcedialog=new SourceDialog(this,dynamic_cast<Source*>(itemAt(event->pos())));
            currentMode=IDLE;
            sourcedialog->exec();
        }
        else
            currentMode=IDLE;

    }
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
        }else{      //unselected
            itemSelected=dynamic_cast<BasicComponent*>(itemAt(event->pos()));
            qDebug()<<"selected12345\n";
        }
        break;
        case CUT:
        {
        BasicComponent *toDelete=dynamic_cast<BasicComponent*>(itemAt(event->pos()));
        Line *lineToDelete=dynamic_cast<Line*>(itemAt(event->pos()));
        if(toDelete){components->removeOne(toDelete);delete toDelete;}
        if(lineToDelete){
            Node *nodeOfLineToDelete=dynamic_cast<Node*>(lineToDelete->group());
            for(int i=0;i<nodeOfLineToDelete->connectedPorts.size();i++){
                //if current lineToDelete connects to a port, then this port should be deleted from node
                if((*(nodeOfLineToDelete->connectedPorts.at(i).second))==lineToDelete->getPoint1()
                        ||(*(nodeOfLineToDelete->connectedPorts.at(i).second))==lineToDelete->getPoint2()){
                    nodeOfLineToDelete->connectedPorts.erase(nodeOfLineToDelete->connectedPorts.begin()+i);
                    i--;
                }
            }
            //delete line
            qDebug() << "delete line";
            lines->removeOne(lineToDelete);
            nodeOfLineToDelete->removeFromGroup(lineToDelete);
            delete lineToDelete;
            //delete the node if none of port exist
            if(nodeOfLineToDelete->connectedPorts.empty()){
                qDebug() << "delete the node";
                for(QGraphicsItem *line:nodeOfLineToDelete->childItems()){
                    qDebug() << "delete lines in node";
                    lines->removeOne(dynamic_cast<Line*>(line));
                    delete line;
                }
                nodes->removeOne(nodeOfLineToDelete);
                delete nodeOfLineToDelete;
            }
            qDebug() << "delete finish";
        }
        break;
        }
        case DRAWLINE:
        {
        QGraphicsItem *itemClicked=nullptr;
        for(QGraphicsItem *i:items(event->pos()))
            if(i!=drawLineX&&i!=drawLineY&&i!=drawingline){ //when drawingline, click through current line
                itemClicked=i;
                break;
            }
        qDebug() << "DrawLineClicked\n";
        if(itemClicked!=nullptr){
            qDebug() << "itemClicked\n";
            BasicComponent *componentClicked=dynamic_cast<BasicComponent*>(itemClicked);
            Line *lineClicked=dynamic_cast<Line*>(itemClicked);
            if(componentClicked){
                qDebug() << "componentClicked\n" << mapToScene(event->pos()) << endl;
                QPoint *portClicked=componentClicked->getPortByScenePoint(mapToScene(event->pos()));
                if(portClicked){    //null if not found
                    qDebug() << "portClicked\n";
                    if(drawingline){
                        //finish the line
                        drawingline->setPoint2(mapToScene(event->pos()));
                        Node *node=dynamic_cast<Node*>(drawingline->group());
                        node->connectedPorts.append(Node::PortOfComponent(componentClicked,portClicked));
                        drawingline=nullptr;
                        scene->update();
                    }else{
                        //create a new node and draw a line
                        Node *newNode=new Node();
                        QPointF pointFClicked=mapToScene(event->pos());
                        drawingline=new Line(pointFClicked,pointFClicked);
                        newNode->addToGroup(drawingline);
                        drawingline->setGroup(newNode);
//                        drawingline->group()
                        nodes->append(newNode);
                        lines->append(drawingline);
                        newNode->connectedPorts.append(Node::PortOfComponent(componentClicked,portClicked));
                        this->drawComponents();
                    }
                }
            }else if(lineClicked){
                qDebug() << "Clicked on Line\n";
            }
        }else{  //itemcliked == nullptr
            if(drawingline){//finish this line and create new line
                Node *node=dynamic_cast<Node*>(drawingline->group());

            }
        }
            break;
        }
    }
    QGraphicsView::mousePressEvent(event);
//    viewport()->setCursor(Qt::CrossCursor);   //override default pan cursor
}
void Workspace::mouseMoveEvent(QMouseEvent *event){
    //for drawLine X Y axis
    if(drawLineX){scene->removeItem(drawLineX);delete drawLineX;drawLineX=nullptr;}
    if(drawLineY){scene->removeItem(drawLineY);delete drawLineY;drawLineY=nullptr;}

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
        viewport()->setCursor(Qt::OpenHandCursor);
        if(itemSelected){
            itemSelected->moveTo(mapToScene(event->x(),event->y()));
        }
        break;
        case DRAWLINE:
    {
        drawLineX=scene->addLine(QLineF(mapToScene(QPoint(0,event->pos().y())),mapToScene(QPoint(viewport()->width()-1,event->pos().y()))),QPen(Qt::DotLine));
        drawLineY=scene->addLine(QLineF(mapToScene(QPoint(event->pos().x(),0)),mapToScene(QPoint(QPoint(event->pos().x(),viewport()->height()-1)))),QPen(Qt::DotLine));
        if(drawingline){
            qreal dx=drawingline->getPoint1().x()*BasicComponent::pixPerAnker-mapToScene(event->pos()).x();
            qreal dy=drawingline->getPoint1().y()*BasicComponent::pixPerAnker-mapToScene(event->pos()).y();
            if(fabs(dx)>fabs(dy)){
                //draw horizontal, keep y
                drawingline->setPoint2(QPointF(mapToScene(event->pos()).x(),drawingline->getPoint1().y()*BasicComponent::pixPerAnker));
            }else{
                //draw vertical, keep x
                drawingline->setPoint2(QPointF(drawingline->getPoint1().x()*BasicComponent::pixPerAnker,mapToScene(event->pos()).y()));
            }
        }
    }
        break;
    }
    scene->update();
//    qDebug() << "viewMove " << event->x() << " , " << event->y() << endl;
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
//    viewport()->setCursor(Qt::CrossCursor);   //override default pan cursor
}

void Workspace::keyPressEvent(QKeyEvent *event){
    if(itemSelected){
        if(event->modifiers()==Qt::ControlModifier&&event->key()==Qt::Key_R){
            itemSelected->rotate();
        }
    }
}
