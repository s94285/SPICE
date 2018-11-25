#ifndef WORKSPACE_H
#define WORKSPACE_H
#include <QGraphicsView>

class Workspace : public QGraphicsView
{
//    Q_OBJECT        //for signal and slots
public:
    explicit Workspace(QWidget *parent = nullptr);
};

#endif // WORKSPACE_H
