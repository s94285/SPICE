#ifndef NODE_H
#define NODE_H
#include <QGraphicsItemGroup>
#include <basiccomponent.h>
class CircuitSimulation;
class Workspace;

class Node : public QGraphicsItemGroup
{
protected:
    typedef QPair<BasicComponent*,QPoint*> PortOfComponent;
    QVector<PortOfComponent> connectedPorts;
public:
    Node(QGraphicsItem * parent = nullptr);
    friend class CircuitSimulation;
    friend class Workspace;
};

#endif // NODE_H
