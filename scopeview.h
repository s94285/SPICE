#ifndef SCOPEVIEW_H
#define SCOPEVIEW_H
#include <QGraphicsView>
#include "linearcomponent.h"
#include "node.h"
#include "source.h"
#include <QLabel>
class ScopeView : public QGraphicsView
{
    Q_OBJECT
    const static int scale=10000;
public:
    explicit ScopeView(QWidget *parent = nullptr);
    void init();
    void setRunTime(double RunTime){runTime=RunTime;}
    QVector<Node*> nodes;
    QVector<LinearComponent*> linearComponents;
    QVector<Source*> sources;
    QHash<void*,double*> points;
    void clear();
    void draw();
    void setScope(Scope* sc){scope=sc;}
    double runTime;
private:
    Scope *scope;
    QGraphicsScene *scene;
    QPixmap *pixmap1,*pixmap2;
    QPainter *painter1,*painter2;
    QGraphicsPixmapItem *item;
    void* lineSelected=nullptr;
    QVector<QAction*> deleteActions,cursorActions;
    void deleteLine(void *);
    void *currentCursorLine=nullptr;
    int cursorX1,cursorX2;
public slots:
    void mouseMoveEvent(QMouseEvent *event);
};

#endif // SCOPEVIEW_H
