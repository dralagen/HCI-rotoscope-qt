#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "newproject.h"
#include <cstdio>
#include <iostream>
#include <QDebug>
#include <QColorDialog>
#include <QFileDialog>
#include <QMessageBox>
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
    v_final_calques= new QVector<QImage>();
    nb_frame=0;
    setWindowTitle("Rotoscope Project");
    background_showed = true;

    disableMainAction(true);
    ui->buttonFirstPicture->setDisabled(true);
    ui->buttonPreviousPicture->setDisabled(true);
    ui->buttonLastPicture->setDisabled(true);
    ui->buttonNextPicture->setDisabled(true);
}

MainWindow::~MainWindow()
{
    delete ui;
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
        freqVideo = newProject.getFrequency().toInt();
        extractPictures(newProject.getMovie(), newProject.getFrequency());

        QImage tmp(outputBasedir+"00001.png");

        QSize s(tmp.size());

        ui->widgetRotoscope->resize(s);
        ui->widgetRotoscope->setRatio((double) s.height() / (double) s.width());

        disableMainAction(false);
        ui->buttonNewFrame->setDisabled(false);
    }
}

void MainWindow::on_actionUndo_triggered()
{
    this->ui->widgetRotoscope->undo();
}

void MainWindow::on_sizeBox_valueChanged(int arg1)
{
     this->ui->widgetRotoscope->setPenSize(arg1);
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
    nb_frame = 1;

    ui->labelMaxPicture->setText("/1");

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
    // save current
    saveCalque(currentCalque);

    currentCalque = i;

    this->ui->inputCurrentPicture->setText(QString::number(currentCalque+1));

    this->ui->widgetRotoscope->setCurrentCalqueNumber(currentCalque);

    this->ui->widgetRotoscope->setCalque(v_final_calques->at(currentCalque));
    this->ui->widgetRotoscope->setDrawingCalques(*v_final_calques);
    this->ui->widgetRotoscope->setBackground(v_path_backgrounds->at(currentCalque));

    bool disabled = (currentCalque < 1);
    this->ui->buttonFirstPicture->setDisabled(disabled);
    this->ui->buttonPreviousPicture->setDisabled(disabled);

    disabled = (currentCalque >= nb_frame-1);
    this->ui->buttonLastPicture->setDisabled(disabled);
    this->ui->buttonNextPicture->setDisabled(disabled);
}


void MainWindow::on_setColorButton_clicked()
{
    QColorDialog cDiag(this);
    QColor c= cDiag.getColor();

    //historique des couleurs ( 6 couleurs )
    switch (v_color->size()) {
    case 1:
        v_color->push_front(c);
        this->ui->colorButton1->setStyleSheet(QString("background-color:%1").arg(v_color->at(0).name()));
        this->ui->colorButton2->setStyleSheet(QString("background-color:%1").arg(v_color->at(1).name()));

    break;
    case 2:
        v_color->push_front(c);
        this->ui->colorButton1->setStyleSheet(QString("background-color:%1").arg(v_color->at(0).name()));
        this->ui->colorButton2->setStyleSheet(QString("background-color:%1").arg(v_color->at(1).name()));
        this->ui->colorButton3->setStyleSheet(QString("background-color:%1").arg(v_color->at(2).name()));

    break;
    case 3:
        v_color->push_front(c);
        this->ui->colorButton1->setStyleSheet(QString("background-color:%1").arg(v_color->at(0).name()));
        this->ui->colorButton2->setStyleSheet(QString("background-color:%1").arg(v_color->at(1).name()));
        this->ui->colorButton3->setStyleSheet(QString("background-color:%1").arg(v_color->at(2).name()));
        this->ui->colorButton4->setStyleSheet(QString("background-color:%1").arg(v_color->at(3).name()));

    break;

    case 4:
        v_color->push_front(c);

        this->ui->colorButton1->setStyleSheet(QString("background-color:%1").arg(v_color->at(0).name()));
        this->ui->colorButton2->setStyleSheet(QString("background-color:%1").arg(v_color->at(1).name()));
        this->ui->colorButton3->setStyleSheet(QString("background-color:%1").arg(v_color->at(2).name()));
        this->ui->colorButton4->setStyleSheet(QString("background-color:%1").arg(v_color->at(3).name()));
        this->ui->colorButton5->setStyleSheet(QString("background-color:%1").arg(v_color->at(4).name()));
    break;

    case 5:
        v_color->push_front(c);

        this->ui->colorButton1->setStyleSheet(QString("background-color:%1").arg(v_color->at(0).name()));
        this->ui->colorButton2->setStyleSheet(QString("background-color:%1").arg(v_color->at(1).name()));
        this->ui->colorButton3->setStyleSheet(QString("background-color:%1").arg(v_color->at(2).name()));
        this->ui->colorButton4->setStyleSheet(QString("background-color:%1").arg(v_color->at(3).name()));
        this->ui->colorButton5->setStyleSheet(QString("background-color:%1").arg(v_color->at(4).name()));
        this->ui->colorButton6->setStyleSheet(QString("background-color:%1").arg(v_color->at(5).name()));
    break;

    case 6:
        v_color->removeLast();
        v_color->push_front(c);
        this->ui->colorButton1->setStyleSheet(QString("background-color:%1").arg(v_color->at(0).name()));
        this->ui->colorButton2->setStyleSheet(QString("background-color:%1").arg(v_color->at(1).name()));
        this->ui->colorButton3->setStyleSheet(QString("background-color:%1").arg(v_color->at(2).name()));
        this->ui->colorButton4->setStyleSheet(QString("background-color:%1").arg(v_color->at(3).name()));
        this->ui->colorButton5->setStyleSheet(QString("background-color:%1").arg(v_color->at(4).name()));
        this->ui->colorButton6->setStyleSheet(QString("background-color:%1").arg(v_color->at(5).name()));
    break;

    default:
        v_color->push_front(c);
        this->ui->colorButton1->setStyleSheet(QString("background-color:%1").arg(v_color->at(0).name()));
        break;
    }


    this->ui->widgetRotoscope->setPenColor(c);

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
    v_path_backgrounds=vTmp;

}

void MainWindow::on_buttonShowBackground_toggled(bool checked)
{
   this->ui->widgetRotoscope->hideBackground(!checked);
    ui->buttonShowBackground->setChecked(checked);
    background_showed=checked;
}

void MainWindow::on_buttonNewFrame_clicked()
{
    if(nb_frame<v_final_calques->size()){
        nb_frame++;

        qDebug() << " newFrame ";

        showCalque(nb_frame-1);

        this->ui->labelMaxPicture->setText("/"+QString::number(nb_frame));

        on_buttonShowBackground_toggled(true);

        if (nb_frame == v_final_calques->size()) {
            this->ui->buttonNewFrame->setDisabled(true);
        }

    }

}

void MainWindow::on_actionExport_to_Pictures_triggered()
{   //affichage d'un endroit ou les sauvegarder.
    qDebug()<< "enregistrement";

    QString exportBasedir = fileDialogOpen(true);

    if (!exportBasedir.isEmpty()) {

        exportPictures(exportBasedir);

        QMessageBox::information(this, "Export successfull", "All pictures has exported into " + exportBasedir);

    }

}

void MainWindow::on_actionExport_to_Film_triggered()
{
    QString exportToFilm = fileDialogOpen(false);

    if (!exportToFilm.isEmpty()) {

        QDir exportBasedir(exportToFilm+".pictures");

        exportPictures(exportBasedir.absolutePath());

        QString cmd = "ffmpeg -y -r " + QString::number(freqVideo) + " -i \""
                + exportBasedir.absolutePath() + "/%05d.png\" \"" + exportToFilm+"\"";

        qDebug() << cmd;

        system(cmd.toStdString().data());

        // clean tmp file
        cmd = "rm -rf \"" + exportBasedir.absolutePath() + "\"";
        system(cmd.toStdString().data());

        QMessageBox::information(this, "Export successfull", "Video has exported at " + exportBasedir.absolutePath());
    }
}

void MainWindow::exportPictures(QString dir)
{
    QDir exportBasedir(dir);
    exportBasedir.mkpath(dir);

    for (int x = 0; x < v_final_calques->size(); ++x) {
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

        v_final_calques->at(x).save(exportBasedir.absolutePath()+"/"+imgStr+".png");
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
    int newPicture = ui->inputCurrentPicture->text().toInt();
    if(newPicture >= 0 && newPicture < nb_frame){

        // on retrouve la dernière version du calque correspondant à l'image.
        qDebug()<< "set du calque et du background : " + QString::number(currentCalque);
        showCalque(newPicture);

   }

}


//navigation

void MainWindow::on_buttonNextPicture_clicked()
{
    if(currentCalque < nb_frame)
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
    showCalque(nb_frame-1);
}



void MainWindow::on_buttonFirstPicture_clicked()
{
    showCalque(0);
}

// TODO an action for stop annimation
void MainWindow::on_buttonPlay_clicked()
{

    ui->centralwidget->setDisabled(true);

    int savedPosition = currentCalque;
    saveCalque(savedPosition);
    int lastNbOfCalqToDraw=this->ui->visibleDrawing->text().toInt();
    on_visibleDrawing_valueChanged(0);

    for (int i = 0; i < nb_frame; ++i) {
        showCalque(i);
        this->ui->widgetRotoscope->repaint();
        usleep(1000000/freqVideo); // 42000 => 24 fps
    }

    showCalque(savedPosition);

    ui->centralwidget->setDisabled(false);
    on_visibleDrawing_valueChanged(lastNbOfCalqToDraw);
}

void MainWindow::initVCalques(){

    for (int x = 0; x < v_path_backgrounds->size(); ++x) {
        QImage * imgTmp = new QImage(3,3,QImage::Format_ARGB32_Premultiplied);
        imgTmp->fill(0);
        imgTmp = new QImage(imgTmp->scaled(this->ui->widgetRotoscope->size()));
        v_final_calques->push_back(* imgTmp);
    }

}

// méthode pour l'enregistrement de l'état d'un calque.

void MainWindow::saveCalque(int i){

    v_final_calques->replace(i,this->ui->widgetRotoscope->getLastCalque());


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

QSize MainWindow::sizeHint() const
{
    return QSize(std::max(ui->widgetRotoscope->width(), ui->widgetTools->width()) + ui->widgetNav->width() + 15,
                 std::max(ui->widgetRotoscope->height(), ui->widgetNav->height()) + ui->widgetTools->height() + 10);
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

void MainWindow::on_visibleDrawing_valueChanged(int arg1)
{
    if(arg1 <= v_final_calques->size()){
        this->ui->widgetRotoscope->setDrawingCalques(* v_final_calques,arg1);
    }
}
