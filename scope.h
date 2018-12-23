#ifndef SCOPE_H
#define SCOPE_H
#include "node.h"
#include <QMainWindow>
#include "linearcomponent.h"
#include "scopeview.h"
namespace Ui {
class Scope;
}

class Scope : public QMainWindow
{
    Q_OBJECT

public:
    explicit Scope(QWidget *parent = nullptr);
    ~Scope();
    friend class CircuitSimulation;
    friend class ScopeView;
private:
    Ui::Scope *ui;
    QLabel *labelVoltage[11],*labelCurrent[11],*labelTime[11];
};

#endif // SCOPE_H
