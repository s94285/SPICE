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
#include "circuitsimulation.h"
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
    case RUN:
        viewport()->setCursor(Qt::PointingHandCursor);
        break;
    }

}
void Workspace::mousePressEvent(QMouseEvent *event){
    QGraphicsItem *itemOnTop=nullptr;
    for(QGraphicsItem *i:items(event->pos()))
        if(dynamic_cast<Node*>(i)==nullptr){ //click through all node
            itemOnTop=i;
            break;
        }
    if(itemOnTop)qDebug() << "Clicked On Item";
    if(dynamic_cast<Node*>(itemOnTop))qDebug() << "Clicked on node";
    if(dynamic_cast<Line*>(itemOnTop))qDebug() << "Clicked on line with node: " << dynamic_cast<Node*>(dynamic_cast<Line*>(itemOnTop)->group());
    if(event->button()==Qt::RightButton&&(itemOnTop==nullptr)&&currentMode!=RUN){
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
    if(event->button()==Qt::RightButton&&itemOnTop!=nullptr)
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
        /*if(dynamic_cast<Resistor*>(itemOnTop))
            {
                dialog=new editDialog(this,dynamic_cast<Resistor*>(itemOnTop));
            }
            else if(dynamic_cast<Inductor*>(itemOnTop))
            {
                dialog=new editDialog(this,dynamic_cast<Inductor*>(itemOnTop));
            }
            else
            {
                dialog=new editDialog(this,dynamic_cast<Capacitor*>(itemOnTop));
            }*/
        if(dynamic_cast<Resistor*>(itemOnTop)!=nullptr||dynamic_cast<Inductor*>(itemOnTop)!=nullptr||dynamic_cast<Capacitor*>(itemOnTop)!=nullptr)
        {
            dialog=new editDialog(this,dynamic_cast<BasicComponent*>(itemOnTop));
            currentMode=IDLE;
            dialog->exec();
        }
        else if(dynamic_cast<Source*>(itemOnTop)!=nullptr)
        {
            sourcedialog=new SourceDialog(this,dynamic_cast<Source*>(itemOnTop));
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
            itemSelected=dynamic_cast<BasicComponent*>(itemOnTop);
            qDebug()<<"selected12345\n";
        }
        break;
    case CUT:
    {
        BasicComponent *toDelete=dynamic_cast<BasicComponent*>(itemOnTop);
        Line *lineToDelete=dynamic_cast<Line*>(itemOnTop);
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
            //                if(nodeOfLineToDelete->connectedPorts.empty()){
            qDebug() << "delete the node";
            for(QGraphicsItem *line:nodeOfLineToDelete->childItems()){
                qDebug() << "delete lines in node";
                lines->removeOne(dynamic_cast<Line*>(line));
                delete line;
            }
            nodes->removeOne(nodeOfLineToDelete);
            delete nodeOfLineToDelete;
            //                }
            qDebug() << "delete finish";
            scene->update();
        }
        break;
    }
    case DRAWLINE:
    {
        QGraphicsItem *itemClicked=nullptr;
        for(QGraphicsItem *i:items(event->pos()))
            if(i!=drawLineX&&i!=drawLineY&&i!=drawingline&&dynamic_cast<Node*>(i)==nullptr){ //when drawingline, click through current line and nodes
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
            }else if(lineClicked){  //need to draw blue rectangle for articulation
                qDebug() << "Clicked on another Line\n";
                if(drawingline==nullptr){    //only accept line for simplicy
                    //add a new line with same node
                    Node *node=dynamic_cast<Node*>(lineClicked->group());
                    if(node){
                        drawingline=new Line(mapToScene(event->pos()),mapToScene(event->pos()));
                        drawingline->setPoint1Rect(true);
                        lines->append(drawingline);
                        node->addToGroup(drawingline);
                        drawingline->setGroup(node);
                        scene->update();
                    }else
                        qDebug() << "didn't get node";
                }
            }
        }else{  //itemcliked == nullptr
            qDebug() << "Clicked outside";
            if(drawingline){//finish this line and create new line
                Node *node=dynamic_cast<Node*>(drawingline->group());
                if(node){
                    drawingline=new Line(drawingline->getPoint2(),drawingline->getPoint2());
                    lines->append(drawingline);
                    node->addToGroup(drawingline);
                    drawingline->setGroup(node);
                    scene->update();
                }else
                    qDebug() << "didn't get node";
            }
        }
        break;
    }
    case RUN:
    {
        Line* line=dynamic_cast<Line*>(itemOnTop);
        LinearComponent* lc=dynamic_cast<LinearComponent*>(itemOnTop);
        Source* source=dynamic_cast<Source*>(itemOnTop);
        if(line){
            Node *n=dynamic_cast<Node*>(line->group());
            if(n)
                circuitSimulation->drawFunction(n);
        }else if(lc){
            circuitSimulation->drawFunction(lc);
        }else if(source){
            circuitSimulation->drawFunction(source);
        }
    }
        break;
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
    qDebug() << "rotate: " << itemSelected;
    if(itemSelected){
        if(event->modifiers()==Qt::ControlModifier&&event->key()==Qt::Key_R){
            qDebug() << "rotate";
            itemSelected->rotate();
        }
    }
}
