#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QPushButton>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void extractPictures(QString movie, QString freq);
    void saveCalque(int i);
    void disableMainAction(bool disable);
    ~MainWindow();

public slots:
    void actionUndo_setDisabled(bool b);
protected:
    void resizeEvent(QResizeEvent *event);
    void paintEvent(QPaintEvent *);

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

    void on_buttonPlay_clicked();

    void on_visibleDrawing_valueChanged(int arg1);

    void on_actionAbout_triggered();

    void on_actionAboutQt_triggered();


    void on_actionExport_to_Film_triggered();

    void on_freqDrawing_valueChanged(int value);

private:
    Ui::MainWindow *ui;

    void exportPictures(QString dir);

    void deleteTmpPictures();
    QString fileDialogOpen(bool directory);
    void showCalque(int i);
    void vPathBackgroundSort();
    void initVCalques();

    QVector<QColor>  * v_color;
    QVector<QString> * v_path_backgrounds;
    int                currentCalque;
    QVector<QImage *>  v_final_calques;
    int                nb_frame;
    bool               background_showed;

    QString outputBasedir = "/tmp/rotoscope/";
    int freqVideo;

    QVector<QPushButton *> buttonsColor;

    bool resized;

};


#endif // MAINWINDOW_H
