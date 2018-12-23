#include "scopeview.h"
#include <cmath>
#include <QGraphicsPixmapItem>
#include <complex>
#include <QDebug>
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
    for(auto i:points)delete points[i];
    points.clear();
    double timescale=runTime/scale;

    double amp;
    double phase;
    double angularFreq;
    double maxVoltage=-INT_MAX,minVoltage=INT_MAX;
    double maxCurrent=-INT_MAX,minCurrent=INT_MAX;
    for(int i=0;i<nodes.size();i++)
    {
        points[nodes[i]]=new double[scale];
        for(int k=0;k<scale;k++)points[nodes[i]][k]=0;
        for(int j=0;j<nodes[i]->voltage.size();j++){
            amp=abs(nodes[i]->voltage[j].first);
            phase=arg(nodes[i]->voltage[j].first);
            angularFreq=nodes[i]->voltage[j].second;
            for(int ti=0;ti<scale;ti++){
                double value=amp*sin(angularFreq*ti*timescale+phase);
                points[nodes[i]][ti]+=value;
                if(maxVoltage<points[nodes[i]][ti])maxVoltage=points[nodes[i]][ti];
                if(minVoltage>points[nodes[i]][ti])minVoltage=points[nodes[i]][ti];
            }
        }
    }
    for(int i=0;i<linearComponents.size();i++)
    {
        points[linearComponents[i]]=new double[scale];
        for(int k=0;k<scale;k++)points[linearComponents[i]][k]=0;
        for(int j=0;j<linearComponents[i]->current.size();j++){
            amp=abs(linearComponents[i]->current[j].first);
            phase=arg(linearComponents[i]->current[j].first);
            angularFreq=linearComponents[i]->current[j].second;
            for(int ti=0;ti<scale;ti++){
                double value=amp*sin(angularFreq*ti*timescale+phase);
                points[linearComponents[i]][ti]+=value;
                if(maxCurrent<points[linearComponents[i]][ti])maxCurrent=points[linearComponents[i]][ti];
                if(minCurrent>points[linearComponents[i]][ti])minCurrent=points[linearComponents[i]][ti];
            }
        }
    }
    for(int i=0;i<sources.size();i++)
    {
        points[sources[i]]=new double[scale];
        for(int k=0;k<scale;k++)points[sources[i]][k]=0;
        for(int j=0;j<sources[i]->current.size();j++){
            amp=abs(sources[i]->current[j].first);
            phase=arg(sources[i]->current[j].first);
            angularFreq=sources[i]->current[j].second;
            for(int ti=0;ti<scale;ti++){
                double value=amp*sin(angularFreq*ti*timescale+phase);
                points[sources[i]][ti]+=value;
                if(maxCurrent<points[sources[i]][ti])maxCurrent=points[sources[i]][ti];
                if(minCurrent>points[sources[i]][ti])minCurrent=points[sources[i]][ti];
            }
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
    QPen diffColor[4]={QPen(Qt::red,1),QPen(Qt::blue,1),QPen(Qt::green,1),QPen(Qt::magenta,1)};
    int currentColor=0;
    for(auto node:nodes){
        painter1->setPen(diffColor[(currentColor++)%4]);
        for(int i=1;i<scale;i++){
            painter1->drawLine(static_cast<int>(1.*(i-1)/scale*pixmap1->width()+0.5),
                               static_cast<int>((maxVoltage-points[node][i-1])/(maxVoltage-minVoltage)*pixmap1->height()),
                    static_cast<int>(1.*i/scale*pixmap1->width()+0.5),
                                static_cast<int>((maxVoltage-points[node][i])/(maxVoltage-minVoltage)*pixmap1->height()));
        }
    }
    for(auto lc:linearComponents){
        painter1->setPen(diffColor[(currentColor++)%4]);
        for(int i=1;i<scale;i++){
            painter1->drawLine(static_cast<int>(1.*(i-1)/scale*pixmap1->width()+0.5),
                               static_cast<int>((maxCurrent-points[lc][i-1])/(maxCurrent-minCurrent)*pixmap1->height()),
                    static_cast<int>(1.*i/scale*pixmap1->width()+0.5),
                    static_cast<int>((maxCurrent-points[lc][i])/(maxCurrent-minCurrent)*pixmap1->height()));
        }
    }
    for(auto sc:sources){
        painter1->setPen(diffColor[(currentColor++)%4]);
        for(int i=1;i<scale;i++){
            painter1->drawLine(static_cast<int>(1.*(i-1)/scale*pixmap1->width()+0.5),
                               static_cast<int>((maxCurrent-points[sc][i-1])/(maxCurrent-minCurrent)*pixmap1->height()),
                    static_cast<int>(1.*i/scale*pixmap1->width()+0.5),
                    static_cast<int>((maxCurrent-points[sc][i])/(maxCurrent-minCurrent)*pixmap1->height()));
        }
    }
    item->setPixmap(*pixmap1);
    //finish drawing
    for(int i=0;i<11;i++){
        scope->labelVoltage[i]->setText(QString::number(minVoltage+(maxVoltage-minVoltage)/10*i));
        scope->labelCurrent[i]->setText(QString::number(minCurrent+(maxCurrent-minCurrent)/10*i));
        scope->labelTime[i]->setText(QString::number(runTime/10*i));
    }
    //update delete menu

    for(int i=0;i<actions.size();i++){
        scope->ui->menuDelete->removeAction(actions[i]);
        delete actions[i];
        actions.remove(i--);
    }
    for(int i=0;i<nodes.size();i++){
        QAction *act=new QAction("Node "+QString::number(i));
        actions.append(act);
        scope->ui->menuDelete->addAction(act);
        lineSelected=nodes[i];
        connect(act,SIGNAL(triggered()),this,SLOT(deleteLine()));
    }
    for(int i=0;i<linearComponents.size();i++){
        QAction *act=new QAction(linearComponents[i]->name);
        actions.append(act);
        scope->ui->menuDelete->addAction(act);
        lineSelected=linearComponents[i];
        connect(act,SIGNAL(triggered()),this,SLOT(deleteLine()));
    }
    for(int i=0;i<sources.size();i++){
        QAction *act=new QAction(sources[i]->name);
        actions.append(act);
        scope->ui->menuDelete->addAction(act);
        lineSelected=sources[i];
        connect(act,SIGNAL(triggered()),this,SLOT(deleteLine()));
    }
}

void ScopeView::deleteLine()
{
    qDebug() << "call delete line";
    if(lineSelected==nullptr)return;
    for(auto i:nodes){
        if(i==lineSelected){
            nodes.removeOne(i);
            break;
        }
    }
    for(auto i:sources){
        if(i==lineSelected){
            sources.removeOne(i);
            break;
        }
    }
    for(auto i:linearComponents){
        if(i==lineSelected){
            linearComponents.removeOne(i);
            break;
        }
    }
    this->clear();
    this->draw();
    qDebug() << "break point";
}
