#include "editdialog.h"
#include "ui_editdialog.h"
#include <QDebug>
#include "resistor.h"
#include "inductor.h"
#include "capacitor.h"
editDialog::editDialog(QWidget *parent,BasicComponent *item) :
    QDialog(parent),
    ui(new Ui::editDialog)
{
    ui->setupUi(this);
    citem=item;
    this->setWindowTitle("Settings: "+citem->getName());
    ui->lineEdit_name->setText(citem->getName());
    ui->lineEdit_value->setText(citem->getValue());
}

editDialog::~editDialog()
{
    delete ui;
}


void editDialog::on_buttonBox_accepted()
{
     qDebug()<<"ok button clicked\n";

     qDebug()<<ui->lineEdit_name->text()<<ui->lineEdit_value->text()<<endl;
     citem->set(ui->lineEdit_name->text(),ui->lineEdit_value->text());
}
