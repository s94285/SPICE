#ifndef NODE_H
#define NODE_H
#include <QGraphicsItemGroup>
#include <basiccomponent.h>
#include <complex>

class CircuitSimulation;
class Workspace;

class Node : public QGraphicsItemGroup
{
protected:
    typedef QPair<BasicComponent*,QPoint*> PortOfComponent;
    QVector<PortOfComponent> connectedPorts;
    typedef  QPair<std::complex<double>,double> VoltageofComponent ;//voltage,angular frequency
    QVector<VoltageofComponent> voltage;
public:
    Node(QGraphicsItem * parent = nullptr);
    friend class CircuitSimulation;
    friend class Workspace;
};

#endif // NODE_H
