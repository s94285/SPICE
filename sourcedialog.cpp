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
    citem->setdata(ui->lineEdit_Amp->text(),ui->lineEdit_Freq->text(),ui->lineEdit_phase->text(),ui->lineEdit_DCValue->text(),ui->lineEdit_Von->text(),ui->lineEdit_Vinitial->text());
}
