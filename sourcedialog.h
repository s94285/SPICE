#ifndef SOURCEDIALOG_H
#define SOURCEDIALOG_H
#include "qabstractbutton.h"
#include <QDialog>
#include "workspace.h"
#include "source.h"

namespace Ui {
class SourceDialog;
}

class SourceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SourceDialog(QWidget *parent = nullptr,Source *item=nullptr);
    ~SourceDialog();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::SourceDialog *ui;
    Source *citem;

};

#endif // SOURCEDIALOG_H
