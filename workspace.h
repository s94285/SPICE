#ifndef WORKSPACE_H
#define WORKSPACE_H
#include <QGraphicsView>
#include "basiccomponent.h"
enum MODE{IDLE,RUN,DRAWLINE,ADDGROUND,ADDRESISTOR,ADDCAPACITOR,ADDINDUCTOR,CUT,MOVE};
class CircuitSimulation;
class Workspace : public QGraphicsView
{
public:
    explicit Workspace(QWidget *parent = nullptr);
    void init();
    void drawComponents(QVector<BasicComponent*> &components_vector);
    friend class CircuitSimulation;
public slots:
    void enterEvent(QEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
private:
    QGraphicsScene *scene;
    QPixmap *pixmap;
    MODE currentMode;
    BasicComponent *itemSelected;
    //for dragging
    bool _pan;
    int _panStartX, _panStartY;
};

#endif // WORKSPACE_H
