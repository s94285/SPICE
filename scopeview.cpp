#include "scopeview.h"
#include <cmath>
#include <QGraphicsPixmapItem>
#include <complex>
#include <QDebug>
#include <qevent.h>
#include "scope.h"
#include "ui_scope.h"
using namespace std;
ScopeView::ScopeView(QWidget *parent):QGraphicsView (parent)
{


}
void ScopeView::init()
{
    scene=new QGraphicsScene();
//    qDebug() << this->geometry().width()-2 << endl << this->geometry().height()-2;
    pixmap1=new QPixmap(this->geometry().width()-2,this->geometry().height()-2);

    pixmap1->fill();
    painter1=new QPainter(pixmap1);


    QPen pen(Qt::red,3);
    painter1->setPen(pen);
    this->setScene(scene);
//    painter1->fillRect(Qt::white);
    item=scene->addPixmap(*pixmap1);
    item->setPixmap(*pixmap1);
    this->show();
    cursorX1=this->geometry().width()/2-50;
    cursorX2=this->geometry().width()/2+50;
//    qDebug() << this->geometry().width()-2 << endl << this->geometry().height()-2;
}

void ScopeView::clear()
{
    delete pixmap1;
    pixmap1=new QPixmap(this->geometry().width()-2,this->geometry().height()-2);
    pixmap1->fill();
    painter1=new QPainter(pixmap1);
    item->setPixmap(*pixmap1);
}

void ScopeView::draw()
{
    double maxVoltage=-INT_MAX,minVoltage=INT_MAX;
    double maxCurrent=-INT_MAX,minCurrent=INT_MAX;
    for(auto i:nodes){
        for(int j=0;j<scale;j++){
            maxVoltage=(maxVoltage<(*points)[i][j])?(*points)[i][j]:maxVoltage;
            minVoltage=(minVoltage>(*points)[i][j])?(*points)[i][j]:minVoltage;
        }
    }
    for(auto i:linearComponents){
        for(int j=0;j<scale;j++){
            maxCurrent=(maxCurrent<(*points)[i][j])?(*points)[i][j]:maxCurrent;
            minCurrent=(minCurrent>(*points)[i][j])?(*points)[i][j]:minCurrent;
        }
    }
    for(auto i:sources){
        for(int j=0;j<scale;j++){
            maxCurrent=(maxCurrent<(*points)[i][j])?(*points)[i][j]:maxCurrent;
            minCurrent=(minCurrent>(*points)[i][j])?(*points)[i][j]:minCurrent;
        }
    }
    if(abs(minVoltage-maxVoltage)<1e-9){
        if(abs(minVoltage-0)<1e-9){ //zero
            minVoltage=-5;
            maxVoltage=5;
        }else{                      //DC
            if(maxVoltage<0)maxVoltage=-maxVoltage;
            if(minVoltage>0)minVoltage=-minVoltage;
        }
    }
    if(abs(minCurrent-maxCurrent)<1e-9){
        if(abs(minCurrent-0)<1e-9){ //zero
            minCurrent=-5;
            maxCurrent=5;
        }else{                      //DC
            if(maxCurrent<0)maxCurrent=-maxCurrent;
            if(minCurrent>0)minCurrent=-minCurrent;
        }
    }
    if(abs(minVoltage)>abs(maxVoltage)){
        maxVoltage=-minVoltage;
    }else{
        minVoltage=-maxVoltage;
    }
    if(abs(minCurrent)>abs(maxCurrent)){
        maxCurrent=-minCurrent;
    }else{
        minCurrent=-maxCurrent;
    }
    minVoltage--;maxVoltage++;minCurrent--;maxCurrent++;
    const int numOfColors=8;
    QPen diffColor[numOfColors]={QPen(Qt::blue,1),QPen(Qt::red,1),QPen(Qt::magenta,1),QPen(Qt::darkGreen,1),QPen(Qt::yellow,1),QPen(Qt::gray,1),QPen(Qt::darkMagenta,1),QPen(Qt::darkBlue,1)};
    int currentColor=0;
    for(int n=0;n<nodes.size();n++){
        painter1->setPen(diffColor[currentColor]);
        for(int i=1;i<scale;i++){
            painter1->drawLine(static_cast<int>(1.*(i-1)/scale*pixmap1->width()+0.5),
                               static_cast<int>((maxVoltage-(*points)[nodes[n]][i-1])/(maxVoltage-minVoltage)*pixmap1->height()),
                    static_cast<int>(1.*i/scale*pixmap1->width()+0.5),
                                static_cast<int>((maxVoltage-(*points)[nodes[n]][i])/(maxVoltage-minVoltage)*pixmap1->height()));
        }
        currentColor=(currentColor+1)%numOfColors;
    }
    for(int l=0;l<linearComponents.size();l++){
        painter1->setPen(diffColor[currentColor]);
        for(int i=1;i<scale;i++){
            painter1->drawLine(static_cast<int>(1.*(i-1)/scale*pixmap1->width()+0.5),
                               static_cast<int>((maxCurrent-(*points)[linearComponents[l]][i-1])/(maxCurrent-minCurrent)*pixmap1->height()),
                    static_cast<int>(1.*i/scale*pixmap1->width()+0.5),
                    static_cast<int>((maxCurrent-(*points)[linearComponents[l]][i])/(maxCurrent-minCurrent)*pixmap1->height()));
        }
        currentColor=(currentColor+1)%numOfColors;
    }
    for(int s=0;s<sources.size();s++){
        painter1->setPen(diffColor[currentColor]);
        for(int i=1;i<scale;i++){
            painter1->drawLine(static_cast<int>(1.*(i-1)/scale*pixmap1->width()+0.5),
                               static_cast<int>((maxCurrent-(*points)[sources[s]][i-1])/(maxCurrent-minCurrent)*pixmap1->height()),
                    static_cast<int>(1.*i/scale*pixmap1->width()+0.5),
                    static_cast<int>((maxCurrent-(*points)[sources[s]][i])/(maxCurrent-minCurrent)*pixmap1->height()));
        }
        currentColor=(currentColor+1)%numOfColors;
    }
    //finish drawing

    //draw legends
    currentColor=0;
    for(int i=0;i<nodes.size();i++){
        painter1->setPen(Qt::black);
        painter1->setBrush(Qt::white);
        QRect toDraw(950,20+22*currentColor,170,20);
        painter1->drawRect(toDraw);
        painter1->drawText(toDraw.topLeft().x()+50,toDraw.topLeft().y()+(toDraw.height()-painter1->fontMetrics().height())/2+painter1->fontMetrics().ascent(),"V: Node "+QString::number(i));
        painter1->setPen(diffColor[currentColor]);
        painter1->drawLine(toDraw.topLeft().x()+2,toDraw.topLeft().y()+toDraw.height()/2,toDraw.topLeft().x()+45,toDraw.topLeft().y()+toDraw.height()/2);
        currentColor=(currentColor+1)%numOfColors;
    }
    for(int i=0;i<linearComponents.size();i++){
        painter1->setPen(Qt::black);
        painter1->setBrush(Qt::white);
        QRect toDraw(950,20+22*currentColor,170,20);
        painter1->drawRect(toDraw);
        painter1->drawText(toDraw.topLeft().x()+50,toDraw.topLeft().y()+(toDraw.height()-painter1->fontMetrics().height())/2+painter1->fontMetrics().ascent(),"I: "+linearComponents[i]->name);
        painter1->setPen(diffColor[currentColor]);
        painter1->drawLine(toDraw.topLeft().x()+2,toDraw.topLeft().y()+toDraw.height()/2,toDraw.topLeft().x()+45,toDraw.topLeft().y()+toDraw.height()/2);
        currentColor=(currentColor+1)%numOfColors;
    }
    for(int i=0;i<sources.size();i++){
            painter1->setPen(Qt::black);
            painter1->setBrush(Qt::white);
            QRect toDraw(950,20+22*currentColor,170,20);
            painter1->drawRect(toDraw);
            painter1->drawText(toDraw.topLeft().x()+50,toDraw.topLeft().y()+(toDraw.height()-painter1->fontMetrics().height())/2+painter1->fontMetrics().ascent(),"I: "+sources[i]->name);
            painter1->setPen(diffColor[currentColor]);
            painter1->drawLine(toDraw.topLeft().x()+2,toDraw.topLeft().y()+toDraw.height()/2,toDraw.topLeft().x()+45,toDraw.topLeft().y()+toDraw.height()/2);
            currentColor=(currentColor+1)%numOfColors;
        }
    //set label
    for(int i=0;i<11;i++){
        if(nodes.size()>0)
            scope->labelVoltage[i]->setText(QString::number(minVoltage+(maxVoltage-minVoltage)/10*i));
        else
            scope->labelVoltage[i]->setText("");
        if(linearComponents.size()>0||sources.size()>0)
            scope->labelCurrent[i]->setText(QString::number(minCurrent+(maxCurrent-minCurrent)/10*i));
        else
            scope->labelCurrent[i]->setText("");
        scope->labelTime[i]->setText(QString::number(runTime/10*i));
    }
    //update delete menu

    for(int i=0;i<deleteActions.size();i++){
        scope->ui->menuDelete->removeAction(deleteActions[i]);
        delete deleteActions[i];
        deleteActions.remove(i--);
    }
    for(int i=0;i<nodes.size();i++){
        QAction *act=new QAction("V: Node "+QString::number(i));
        deleteActions.append(act);
        scope->ui->menuDelete->addAction(act);
        connect(act,&QAction::triggered,this,[this,i]{deleteLine(nodes[i]);});  //use lambda functor to pass parameter
    }
    for(int i=0;i<linearComponents.size();i++){
        QAction *act=new QAction("I: "+linearComponents[i]->name);
        deleteActions.append(act);
        scope->ui->menuDelete->addAction(act);
        connect(act,&QAction::triggered,this,[this,i]{deleteLine(linearComponents[i]);});
    }
    for(int i=0;i<sources.size();i++){
        QAction *act=new QAction("I: "+sources[i]->name);
        deleteActions.append(act);
        scope->ui->menuDelete->addAction(act);
        connect(act,&QAction::triggered,this,[this,i]{deleteLine(sources[i]);});
    }

    //update cursor menu
    for(int i=0;i<cursorActions.size();i++){
        scope->ui->menuCursor->removeAction(cursorActions[i]);
        delete cursorActions[i];
        cursorActions.remove(i--);
    }
    for(int i=0;i<nodes.size();i++){
        QAction *act=new QAction("V: Node "+QString::number(i));
        cursorActions.append(act);
        scope->ui->menuCursor->addAction(act);
        connect(act,&QAction::triggered,this,[this,i]{currentCursorLine=nodes[i];this->clear();this->draw();});  //use lambda functor to pass parameter
    }
    for(int i=0;i<linearComponents.size();i++){
        QAction *act=new QAction("I: "+linearComponents[i]->name);
        cursorActions.append(act);
        scope->ui->menuCursor->addAction(act);
        connect(act,&QAction::triggered,this,[this,i]{currentCursorLine=(linearComponents[i]);this->clear();this->draw();});
    }
    for(int i=0;i<sources.size();i++){
        QAction *act=new QAction("I: "+sources[i]->name);
        cursorActions.append(act);
        scope->ui->menuCursor->addAction(act);
        connect(act,&QAction::triggered,this,[this,i]{currentCursorLine=(sources[i]);this->clear();this->draw();});
    }
    //add delete cursor action
    {
        QAction *act=new QAction("Delete Cursor");
        cursorActions.append(act);
        scope->ui->menuCursor->addAction(act);
        connect(act,&QAction::triggered,this,[this]{currentCursorLine=(nullptr);this->clear();this->draw();});
    }
    //draw cursor
    if(currentCursorLine!=nullptr){
        QPen oldPen=painter1->pen();
        painter1->setPen(QPen(QBrush(Qt::black),1,Qt::DotLine));
        painter1->drawLine(cursorX1,0,cursorX1,viewport()->height()-1);
        painter1->drawLine(cursorX2,0,cursorX2,viewport()->height()-1);
        currentColor=0;
        bool find=false;
        for(auto i:nodes){
            if(static_cast<void*>(i)==currentCursorLine){find=true;break;}
            currentColor++;
        }
        if(!find)
            for(auto i:linearComponents){
                if(static_cast<void*>(i)==currentCursorLine){find=true;break;}
                currentColor++;
            }
        if(!find)
            for(auto i:sources){
                if(static_cast<void*>(i)==currentCursorLine){find=true;break;}
                currentColor++;
            }
        painter1->setPen(diffColor[currentColor%numOfColors]);
        painter1->drawText(cursorX1,painter1->fontMetrics().ascent()+10,"1");
        painter1->drawText(cursorX2,painter1->fontMetrics().ascent()+10,"2");
        painter1->setPen(Qt::black);
        painter1->setBrush(Qt::white);
        QRect toDraw(950,viewport()->height()-10-(painter1->fontMetrics().height()+5)*6,170,(painter1->fontMetrics().height()+5)*6+8);
        painter1->drawRect(toDraw);
        double x1,x2,dx,y1,y2,dy;
        painter1->drawText(toDraw.topLeft().x()+5,toDraw.topLeft().y()+5+painter1->fontMetrics().ascent(),"x1: "+QString::number(x1=runTime*cursorX1/pixmap1->width())+" s");
        painter1->drawText(toDraw.topLeft().x()+5,toDraw.topLeft().y()+(5+painter1->fontMetrics().ascent())*2,"x2: "+QString::number(x2=runTime*cursorX2/pixmap1->width())+" s");
        painter1->drawText(toDraw.topLeft().x()+5,toDraw.topLeft().y()+(5+painter1->fontMetrics().ascent())*3,"dx: "+QString::number(dx=(x2-x1))+" s");
        painter1->drawText(toDraw.topLeft().x()+5,toDraw.topLeft().y()+(5+painter1->fontMetrics().ascent())*4,"y1: "+QString::number(y1=(*points)[currentCursorLine][10000*cursorX1/pixmap1->width()]));
        painter1->drawText(toDraw.topLeft().x()+5,toDraw.topLeft().y()+(5+painter1->fontMetrics().ascent())*5,"y2: "+QString::number(y2=(*points)[currentCursorLine][10000*cursorX2/pixmap1->width()]));
        painter1->drawText(toDraw.topLeft().x()+5,toDraw.topLeft().y()+(5+painter1->fontMetrics().ascent())*6,"dy: "+QString::number(dy=(y2-y1)));
        painter1->drawText(toDraw.topLeft().x()+5,toDraw.topLeft().y()+(5+painter1->fontMetrics().ascent())*7,"dy/dx: "+QString::number(dy/dx));

        painter1->setPen(oldPen);
    }
    //update pixmapitem
    item->setPixmap(*pixmap1);
}

void ScopeView::deleteLine(void *toDelete)
{
    qDebug() << "call delete line";
    if(toDelete==nullptr)return;
    for(auto i:nodes){
        if(i==toDelete){
            nodes.removeOne(i);
            break;
        }
    }
    for(auto i:sources){
        if(i==toDelete){
            sources.removeOne(i);
            break;
        }
    }
    for(auto i:linearComponents){
        if(i==toDelete){
            linearComponents.removeOne(i);
            break;
        }
    }
    this->clear();
    this->draw();
    //    qDebug() << "break point";
}

void ScopeView::mouseMoveEvent(QMouseEvent *event)
{
    //only called when dragging in scopeView
    if(currentCursorLine&&event->pos().x()>=0&&event->pos().x()<viewport()->width()){
        //update cursor location
        ((abs(cursorX1-event->pos().x())<=abs(cursorX2-event->pos().x()))
                ?cursorX1:cursorX2)=event->pos().x();
        this->clear();
        this->draw();
    }
}

