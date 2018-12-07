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
}

editDialog::~editDialog()
{
    delete ui;
}


void editDialog::on_buttonBox_accepted()
{
     qDebug()<<"ok button clicked\n";
    /* if(dynamic_cast<Resistor *>(citem))
     {

     }
     else if(dynamic_cast<Inductor *>(citem))
     {

     }
     else
     {

     }*/
     qDebug()<<ui->lineEdit_2->text()<<ui->lineEdit->text()<<endl;
     citem->set(ui->lineEdit->text(),ui->lineEdit_2->text());
}

void editDialog::on_buttonBox_clicked(QAbstractButton *button)
{

}
