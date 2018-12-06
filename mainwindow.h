#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
class CircuitSimulation;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    friend class CircuitSimulation;
private:
    Ui::MainWindow *ui;
    QPushButton *toolBarButton[9];
};

#endif // MAINWINDOW_H
