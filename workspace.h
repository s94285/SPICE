#ifndef WORKSPACE_H
#define WORKSPACE_H
#include <QGraphicsView>
#include "basiccomponent.h"
class CircuitSimulation;
class Workspace : public QGraphicsView
{
public:
    explicit Workspace(QWidget *parent = nullptr);
    void init();
    void drawComponents(QVector<BasicComponent*> &components_vector);
signals:
public slots:
    void enterEvent(QEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
private:
    QGraphicsScene *scene;
    QPixmap *pixmap;
    //for dragging
    bool _pan;
    int _panStartX, _panStartY;
};

#endif // WORKSPACE_H
