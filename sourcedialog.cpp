#include "sourcedialog.h"
#include "ui_sourcedialog.h"
#include <QDebug>
SourceDialog::SourceDialog(QWidget *parent,Source *item) :
    QDialog(parent),
    ui(new Ui::SourceDialog)
{
    QString getamp,getfreq,getphase,getDCValue,getVon,getVinitial;
    ui->setupUi(this);
    citem=item;
    switch (citem->currentMode) {
    case DC:ui->radioButton_DC->setChecked(true);break;
    case SIN:ui->radioButton_SIN->setChecked(true);break;
    case SQUARE:ui->radioButton_Square->setChecked(true);break;
    }
    this->setWindowTitle("Settings: "+citem->getName());
    ui->lineEdit_Name->setText(citem->getName());
    citem->getdata(getamp,getfreq,getphase,getDCValue,getVon,getVinitial);
    ui->lineEdit_DCValue->setText(getDCValue);
    ui->lineEdit_Amp->setText(getamp);
    ui->lineEdit_Freq->setText(getfreq);
    ui->lineEdit_Vinitial->setText(getVinitial);
    ui->lineEdit_Von->setText(getVon);
    ui->lineEdit_phase->setText(getphase);
}

SourceDialog::~SourceDialog()
{
    delete ui;
}

void SourceDialog::on_buttonBox_accepted()
{

    if(ui->radioButton_DC->isChecked())
    {
        citem->currentMode=DC;
        citem->set(ui->lineEdit_Name->text(),ui->lineEdit_DCValue->text());
        citem->setdata(ui->lineEdit_Amp->text(),ui->lineEdit_Freq->text(),ui->lineEdit_phase->text(),ui->lineEdit_DCValue->text(),ui->lineEdit_Von->text(),ui->lineEdit_Vinitial->text());
    }
    else if(ui->radioButton_SIN->isChecked())
    {
        citem->set(ui->lineEdit_Name->text(),ui->lineEdit_Amp->text()+" "+ui->lineEdit_Freq->text()+" "+ui->lineEdit_phase->text()+"°");
        citem->currentMode=SIN;
        citem->setdata(ui->lineEdit_Amp->text(),ui->lineEdit_Freq->text(),ui->lineEdit_phase->text(),ui->lineEdit_DCValue->text(),ui->lineEdit_Von->text(),ui->lineEdit_Vinitial->text());
    }
    else
    {
        citem->set(ui->lineEdit_Name->text(),ui->lineEdit_Vinitial->text()+" "+ui->lineEdit_Von->text()+" "+ui->lineEdit_Freq->text());
        citem->currentMode=SQUARE;
        citem->setdata(ui->lineEdit_Amp->text(),ui->lineEdit_Freq->text(),ui->lineEdit_phase->text(),ui->lineEdit_DCValue->text(),ui->lineEdit_Von->text(),ui->lineEdit_Vinitial->text());
    }


}
