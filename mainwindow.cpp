#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "newproject.h"
#include <cstdio>
#include <iostream>
#include <QDebug>
#include <QColorDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QEvent>
#include <QShortcut>
#include <dirent.h>
#include <unistd.h>

#define TOOL_ERASER 1
#define TOOL_BRUSH 2

// !NOTE! : this->ui->widgetRotoscope = DrawingArea !

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    v_color = new QVector<QColor>();
    v_path_backgrounds = new QVector<QString>();
    currentCalque= -1;

    setWindowTitle("Rotoscope Project");
    background_showed = true;

    disableMainAction(true);
    ui->buttonFirstPicture->setDisabled(true);
    ui->buttonPreviousPicture->setDisabled(true);
    ui->buttonLastPicture->setDisabled(true);
    ui->buttonNextPicture->setDisabled(true);

    // add reference of colorButton in buttonsColor
    buttonsColor.push_back(ui->colorButton1);
    buttonsColor.push_back(ui->colorButton2);
    buttonsColor.push_back(ui->colorButton3);
    buttonsColor.push_back(ui->colorButton4);
    buttonsColor.push_back(ui->colorButton5);
    buttonsColor.push_back(ui->colorButton6);

    ui->widgetRotoscope->setDrawingCalques(&v_final_calques);

    playCalques = nullptr;

    connect(ui->widgetRotoscope, SIGNAL(undoDisabled(bool)), this, SLOT(actionUndo_setDisabled(bool)));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *)
{
    resized = true;
}

void MainWindow::paintEvent(QPaintEvent *)
{

    if (resized) {
        resized = false;

        if (v_final_calques.empty()) {
            return ;
        }

        ui->widgetRotoscope->setBackground(v_path_backgrounds->at(currentCalque));

        QSize bgSize = ui->widgetRotoscope->getBackgroundSize();

        for (int i = 0; i < v_final_calques.size(); ++i) {
            QImage *tmp = v_final_calques.at(i);
            v_final_calques.replace(i, new QImage(tmp->scaled(bgSize)));
            delete tmp;
        }

        ui->widgetRotoscope->setCalque(v_final_calques.at(currentCalque));
    }
}

void MainWindow::on_actionExit_triggered()
{
    exit(1);
}

MAINWINDOW_H
void MainWindow::on_actionNew_Project_triggered()
{
    NewProject newProject;
    newProject.exec();
    if (newProject.result() == QDialog::Accepted && newProject.getMovie() != "") {

        QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

        freqVideo = newProject.getFrequency().toInt();

        extractPictures(newProject.getMovie(), newProject.getFrequency());


        QImage tmp(outputBasedir+"00001.png");

        QSize s(tmp.size());

        QSize sHint(s.width() + ui->widgetTools->width(),
                    s.height() + ui->widgetNav->height() + ui->menubar->height() + 30);

        resize(sHint);

        QSizePolicy qsp(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
        qsp.setHeightForWidth(true);
        setSizePolicy(qsp);

        disableMainAction(false);

        on_sizeBox_valueChanged(1);

        QApplication::restoreOverrideCursor();
    }
}

void MainWindow::on_actionUndo_triggered()
{
    this->ui->widgetRotoscope->undo();
}

void MainWindow::actionUndo_setDisabled(bool b)
{
    ui->actionUndo->setDisabled(b);
}

void MainWindow::on_sizeBox_valueChanged(int penSize)
{
    this->ui->widgetRotoscope->setPenSize(penSize);

}

void MainWindow::on_brushButton_clicked()
{
    this->ui->widgetRotoscope->setTooltype(TOOL_BRUSH);
    this->ui->brushButton->setChecked(true);
    this->ui->eraserButton->setChecked(false);
}

void MainWindow::on_eraserButton_clicked()
{
    this->ui->widgetRotoscope->setTooltype(TOOL_ERASER);
    this->ui->brushButton->setChecked(false);
    this->ui->eraserButton->setChecked(true);
}

void MainWindow::extractPictures(QString movie, QString frequency){

    // nouveau projet  => suppression des images extraite d'un ancien projet
    deleteTmpPictures();

   QString command="ffmpeg -y -i ";
   QString fpsOption=" -r ";
   QString outputFormat = " -f image2 ";
   QString output ="%05d.png";

   QString extractCommand= command+"\""+movie+"\""+fpsOption+frequency+outputFormat+"\""+outputBasedir+output+"\"";
   qDebug() << extractCommand;

   // extraction des images par le system
   system(extractCommand.toStdString().data());

   // récupération des nom d'images,
   // ATTENTION :les images sont pris dans le désordre

   DIR * rep = opendir(outputBasedir.toStdString().data());

   if (rep != NULL)
   {
       struct dirent * ent;

       v_path_backgrounds->clear();

       while ((ent = readdir(rep)) != NULL)
       {

           if(strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0){
                QString pathtmp = ent->d_name;
                v_path_backgrounds->push_back(outputBasedir+pathtmp);
           }
       }

       closedir(rep);
   }
   // trie des images pour quelles soient dans le bonne ordre;
   vPathBackgroundSort();

   //on initialise le vecteur de calque avec des calques vides
    initVCalques();

    currentCalque = 0;

    ui->labelMaxPicture->setText("/"+QString::number(v_final_calques.size()));

    showCalque(0);

}

void MainWindow::deleteTmpPictures(){

    //suppression du dossier d'image qui peuvent contenir d'anciennes images extraite
    system(QString("rm -Rf "+outputBasedir).toStdString().data());
    //recréation du dossier d'image vide
    system(QString("mkdir "+outputBasedir).toStdString().data());
}

QString MainWindow::fileDialogOpen(bool directory)
{
    QFileDialog fileDialog;
    fileDialog.setWindowTitle("Export Picture");
    if (directory) {
        fileDialog.setFileMode(QFileDialog::Directory);
        fileDialog.setOption(QFileDialog::ShowDirsOnly, true);
    }

    fileDialog.setAcceptMode(QFileDialog::AcceptSave);

    if (fileDialog.exec()) {
        return fileDialog.selectedFiles().first();

    }

    return "";
}

void MainWindow::showCalque(int i)
{
    currentCalque = i;

    this->ui->inputCurrentPicture->setText(QString::number(currentCalque+1));

    this->ui->widgetRotoscope->setCurrentCalqueNumber(currentCalque);

    this->ui->widgetRotoscope->setCalque(v_final_calques.at(currentCalque));
    this->ui->widgetRotoscope->setBackground(v_path_backgrounds->at(currentCalque));

    bool disabled = (currentCalque < 1);
    this->ui->buttonFirstPicture->setDisabled(disabled);
    this->ui->buttonPreviousPicture->setDisabled(disabled);

    disabled = (currentCalque >= v_final_calques.size()-1);
    this->ui->buttonLastPicture->setDisabled(disabled);
    this->ui->buttonNextPicture->setDisabled(disabled);

    this->ui->widgetRotoscope->update();
    update();
}


void MainWindow::on_setColorButton_clicked()
{
    QColorDialog cDiag(this);
    cDiag.exec();

    if (cDiag.result() != QDialog::Accepted) {
        return ;
    }

    QColor c= cDiag.selectedColor();

    v_color->removeAll(c);

    v_color->push_front(c);
    this->ui->widgetRotoscope->setPenColor(c);

    if (v_color->size() > buttonsColor.size()) {
        v_color->removeLast();
    }

    //historique des couleurs ( 6 couleurs )
    for (int i = 0; i < v_color->size(); ++i) {
        buttonsColor.at(i)->setStyleSheet(QString("background-color:%1").arg(v_color->at(i).name()));
    }
}

void MainWindow::vPathBackgroundSort(){
       QVector<QString> * vTmp = new QVector<QString>();
       int img=1;

    while(img != v_path_backgrounds->size()+1){
          //todo revoir cette partie !
            QString imgStr = "";
          if(img<10){
               imgStr= "0000"+QString::number(img);
          }else if(img<100){
              imgStr= "000"+QString::number(img);
          }else if(img<1000){
               imgStr= "00"+QString::number(img);
          }else if(img<10000){
              imgStr= "0"+QString::number(img);
          }else{
               imgStr= ""+QString::number(img);
          }

        for (int x = 0; x < v_path_backgrounds->size() ; ++x) {

            if(v_path_backgrounds->at(x).contains(imgStr)){

                vTmp->push_back(v_path_backgrounds->at(x));
             }
        }
        img++;
    }
    delete v_path_backgrounds;
    v_path_backgrounds=vTmp;

}

void MainWindow::on_buttonShowBackground_toggled(bool checked)
{
   this->ui->widgetRotoscope->hideBackground(!checked);
    ui->buttonShowBackground->setChecked(checked);
    background_showed=checked;
}

void MainWindow::on_actionExport_to_Pictures_triggered()
{   //affichage d'un endroit ou les sauvegarder.
    qDebug()<< "enregistrement";

    QString exportBasedir = fileDialogOpen(true);

    if (!exportBasedir.isEmpty()) {

        QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

        exportPictures(exportBasedir);

        QApplication::restoreOverrideCursor();

        QMessageBox::information(this, "Export successfull", "All pictures has exported into " + exportBasedir);

    }

}

void MainWindow::on_actionExport_to_Film_triggered()
{
    QString exportToFilm = fileDialogOpen(false);

    if (!exportToFilm.isEmpty()) {

        QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

        QDir exportBasedir(exportToFilm+".pictures");

        exportPictures(exportBasedir.absolutePath());

        QString cmd = "ffmpeg -y -r " + QString::number(freqVideo) + " -i \""
                + exportBasedir.absolutePath() + "/%05d.png\" \"" + exportToFilm+"\"";

        qDebug() << cmd;

        system(cmd.toStdString().data());

        // clean tmp file
        cmd = "rm -rf \"" + exportBasedir.absolutePath() + "\"";
        system(cmd.toStdString().data());

        QApplication::restoreOverrideCursor();

        QMessageBox::information(this, "Export successfull", "Video has exported at " + exportToFilm);

    }
}

void MainWindow::exportPictures(QString dir)
{
    QDir exportBasedir(dir);
    exportBasedir.mkpath(dir);

    for (int x = 0; x < v_final_calques.size(); ++x) {
        QString imgStr;
        if(x<10){
             imgStr= "0000"+QString::number(x);
        }else if(x<100){
            imgStr= "000"+QString::number(x);
        }else if(x<1000){
             imgStr= "00"+QString::number(x);
        }else if(x<10000){
            imgStr= "0"+QString::number(x);
        }else{
             imgStr= ""+QString::number(x);
        }

        v_final_calques.at(x)->save(exportBasedir.absolutePath()+"/"+imgStr+".png");
    }
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(this, "Rotoscope", "Authors : Garandel Adrien and Perrai David \n"
                                          "License : GNU GPL");
}

void MainWindow::on_actionAboutQt_triggered()
{
    QMessageBox::aboutQt(this, "Rotoscope");
}


void MainWindow::on_inputCurrentPicture_editingFinished()
{
    int newPicture = std::max(std::min(ui->inputCurrentPicture->text().toInt(), v_final_calques.size()) - 1, 0);

    // on retrouve la dernière version du calque correspondant à l'image.
    showCalque(newPicture);

}


//navigation

void MainWindow::on_buttonNextPicture_clicked()
{
    if(currentCalque < v_final_calques.size())
    {
        qDebug() << "next Picture";
        showCalque(currentCalque+1);
    }

}

void MainWindow::on_buttonPreviousPicture_clicked()
{
    if(currentCalque > 0){

        qDebug() << "previous picture";
        showCalque(currentCalque-1);
    }


}

void MainWindow::on_buttonLastPicture_clicked()
{
    showCalque(v_final_calques.size()-1);
}



void MainWindow::on_buttonFirstPicture_clicked()
{
    showCalque(0);
}

// TODO an action for stop annimation
void MainWindow::on_buttonPlay_clicked()
{

    ui->centralwidget->setDisabled(true);

    int *savedPosition = new int(currentCalque);
    int *lastNbOfCalqToDraw = new int(ui->visibleDrawing->text().toInt());
    on_visibleDrawing_valueChanged(0);

    class PlayCalques: public QThread {
    private :
        MainWindow * parentWidget;

    public:
        PlayCalques(MainWindow * parent) {
            parentWidget = parent;
        }

        void run() Q_DECL_OVERRIDE {

            for (int i = 0; i < parentWidget->v_final_calques.size(); ++i) {
                parentWidget->showCalque(i);
                //parentWidget->ui->widgetRotoscope->repaint();
                QThread::msleep(1000/parentWidget->freqVideo);
            }

        }

    };

    playCalques = new PlayCalques(this);

    QShortcut * shortcut = new QShortcut(QKeySequence(Qt::Key_Escape), this);
    QObject::connect(shortcut, &QShortcut::activated, [=]() {
        qDebug() << "stop";
        if (playCalques->isRunning()) {
            playCalques->terminate();
        }

    });

    connect(playCalques, &PlayCalques::finished, [=]() {

        qDebug() << "play finished";
        if (shortcut != NULL) {
            delete shortcut;
        }

        ui->centralwidget->setDisabled(false);
        on_visibleDrawing_valueChanged(*lastNbOfCalqToDraw);

        showCalque(*savedPosition);

        if (savedPosition != NULL) {
            delete savedPosition;
        }

        if (lastNbOfCalqToDraw != NULL) {
            delete lastNbOfCalqToDraw;
        }

        update();

    });

    playCalques->start();

}

void MainWindow::initVCalques(){

    v_final_calques.clear();

    for (int x = 0; x < v_path_backgrounds->size(); ++x) {
        QImage * imgTmp = new QImage(ui->widgetRotoscope->size(),QImage::Format_ARGB32_Premultiplied);
        imgTmp->fill(0);
        v_final_calques.push_back(imgTmp);
    }
}

void MainWindow::disableMainAction(bool disable)
{
    ui->menuExport->setDisabled(disable);
    ui->actionOpen->setDisabled(true); // TODO impl Open project
    ui->actionSave->setDisabled(true || disable); // TODO impl Save project
    ui->actionSave_As->setDisabled(true || disable); // TODO impl Save project as
    ui->actionUndo->setDisabled(disable);
    ui->centralwidget->setDisabled(disable);
}

void MainWindow::on_colorButton1_clicked()
{   if(!v_color->isEmpty()){
        this->ui->widgetRotoscope->setPenColor(v_color->at(0));
      }
}


void MainWindow::on_colorButton2_clicked()
{  if(v_color->size()>=2){
        this->ui->widgetRotoscope->setPenColor(v_color->at(1));
    }
}

void MainWindow::on_colorButton3_clicked()
{  if(v_color->size()>=3){
        this->ui->widgetRotoscope->setPenColor(v_color->at(2));
    }
}

void MainWindow::on_colorButton4_clicked()
{ if(v_color->size()>=4){
        this->ui->widgetRotoscope->setPenColor(v_color->at(3));
    }
}

void MainWindow::on_colorButton5_clicked()
{   if(v_color->size()>=5){
        this->ui->widgetRotoscope->setPenColor(v_color->at(4));
    }
}

void MainWindow::on_colorButton6_clicked()
{  if(v_color->size()>=6){
     this->ui->widgetRotoscope->setPenColor(v_color->at(5));
    }
}

void MainWindow::on_visibleDrawing_valueChanged(int value)
{
    if(value <= v_final_calques.size()){
        ui->widgetRotoscope->setNumberOfCalqueToDraw(value);
        ui->widgetRotoscope->update();
    }
}

void MainWindow::on_freqDrawing_valueChanged(int value)
{
    ui->widgetRotoscope->setFreqDrawindCalques(value);
    ui->freqDrawing->setValue(ui->widgetRotoscope->getFreqDrawingCalques());
}
