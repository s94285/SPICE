#ifndef WORKSPACE_H
#define WORKSPACE_H
#include <QGraphicsView>
#include "basiccomponent.h"
#include <QDialog>
enum MODE{IDLE,RUN,DRAWLINE,ADDGROUND,ADDRESISTOR,ADDCAPACITOR,ADDINDUCTOR,CUT,MOVE};
class CircuitSimulation;
class editDialog;
class Workspace : public QGraphicsView
{
public:
    explicit Workspace(QWidget *parent = nullptr);
    void init();
    void setComponents(QVector<BasicComponent*> &);
    void drawComponents();
    friend class CircuitSimulation;
public slots:
    void enterEvent(QEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
private:
    QGraphicsScene *scene;
    QPixmap *pixmap;
    MODE currentMode;
    BasicComponent *itemSelected;
    QVector<BasicComponent*> *components;
    //for dragging
    bool _pan;
    int _panStartX, _panStartY;
    editDialog *dialog;
};

#endif // WORKSPACE_H
