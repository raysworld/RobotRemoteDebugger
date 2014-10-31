#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pbGetCoord_clicked();

    void on_sbSpeed_valueChanged(int arg1);

    void on_pbSetJoint_clicked();

    void on_pbSetCartesian_clicked();

    void on_pbRDOOn_clicked();

    void on_pbRDOOff_clicked();

    void on_sbStep_valueChanged(int arg1);

    void on_pbJ1pos_clicked();

    void on_pbJ1neg_clicked();

    void on_pbJ2pos_clicked();

    void on_pbJ2neg_clicked();

    void on_pbJ3pos_clicked();

    void on_pbJ3neg_clicked();

    void on_pbJ4pos_clicked();

    void on_pbJ4neg_clicked();

    void on_pbJ5pos_clicked();

    void on_pbJ5neg_clicked();

    void on_pbJ6pos_clicked();

    void on_pbJ6neg_clicked();

    void on_actionIP_Config_triggered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
