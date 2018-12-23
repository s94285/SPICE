#include "scope.h"
#include "ui_scope.h"
Scope::Scope(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Scope)
{
    ui->setupUi(this);
    ui->scopeView->init();

}

Scope::~Scope()
{
    delete ui;
}
