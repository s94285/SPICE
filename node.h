#ifndef NODE_H
#define NODE_H
#include <QGraphicsItemGroup>
#include <basiccomponent.h>
#include <complex>

class CircuitSimulation;
class Workspace;
class Scope;

class Node : public QGraphicsItemGroup
{
protected:
    typedef QPair<BasicComponent*,QPoint*> PortOfComponent;
    QVector<PortOfComponent> connectedPorts;
    typedef  QPair<std::complex<double>,double> VoltageofComponent;//voltage,angular frequency

public:
    Node(QGraphicsItem * parent = nullptr);
    QVector<VoltageofComponent> voltage;
    friend class CircuitSimulation;
    friend class Workspace;
    friend class Scope;
};

#endif // NODE_H
