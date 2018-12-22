#ifndef EDITDIALOG_H
#define EDITDIALOG_H
#include "qabstractbutton.h"
#include <QDialog>
#include "workspace.h"
namespace Ui {
class editDialog;
}

class editDialog : public QDialog
{
    Q_OBJECT
public:
    explicit editDialog(QWidget *parent = nullptr,BasicComponent *item=nullptr);
    ~editDialog();
    void set();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::editDialog *ui;
    BasicComponent *citem;


};

#endif // EDITDIALOG_H
