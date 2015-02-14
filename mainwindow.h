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
    QVector<QColor> * v_color;

    explicit MainWindow(QWidget *parent = 0);
    void extractPictures(QString movie,QString freq);

    ~MainWindow();

private slots:
    void on_actionExit_triggered();

    void on_actionNew_Project_triggered();

    void on_actionUndo_triggered();

    void on_sizeBox_valueChanged(int arg1);

    void on_brushButton_clicked();

    void on_eraserButton_clicked();

    void on_setColorButton_clicked();


private:
    Ui::MainWindow *ui;
};


#endif // MAINWINDOW_H
