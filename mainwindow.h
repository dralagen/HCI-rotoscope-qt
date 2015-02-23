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
    QVector<QColor>  * v_color;
    QVector<QString> * v_path_backgrounds;
    int                current_background;
    QVector<QImage>  * v_final_calques;

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

    void on_buttonShowBackground_toggled(bool checked);

    void on_buttonNewFrame_clicked();

    void on_actionExport_to_Pictures_triggered();

    void on_buttonNextPicture_clicked();

    void on_colorButton1_clicked();

    void on_colorButton2_clicked();

    void on_colorButton3_clicked();

    void on_colorButton4_clicked();

    void on_colorButton5_clicked();

    void on_colorButton6_clicked();

    void on_inputCurrentPicture_editingFinished();

    void on_buttonLastPicture_clicked();

    void on_buttonFirstPicture_clicked();

    void on_buttonPreviousPicture_clicked();

private:
    Ui::MainWindow *ui;
    void deleteTmpPictures();
    void vPathBackgroundSort();

    QString outputBasedir = "/tmp/rotoscope/";
};


#endif // MAINWINDOW_H
