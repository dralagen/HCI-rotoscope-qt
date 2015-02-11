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
    void on_actionExit_triggered();

    void on_actionNew_Project_triggered();

    void on_actionUndo_triggered();

    void on_sizeBox_valueChanged(int arg1);

private:
    Ui::MainWindow *ui;
};


#endif // MAINWINDOW_H
