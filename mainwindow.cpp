#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "newproject.h"
#include <cstdio>
#include <iostream>
#include <QDebug>
#include <QColorDialog>
#include <dirent.h>

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
    current_background= 0;
    v_final_calques= new QVector<QImage>();
    nb_frame=1;
    setWindowTitle("Rotoscope Project");
    background_showed = true;
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
    if (newProject.result() == QDialog::Accepted) {
        extractPictures(newProject.getMovie(),newProject.getFrequency());
    }
    else {
        std::cout << "exit" << std::endl;
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
}

void MainWindow::on_eraserButton_clicked()
{
    this->ui->widgetRotoscope->setTooltype(TOOL_ERASER);
}

void MainWindow::extractPictures(QString movie,QString frequency){

    // nouveau projet  => suppression des images extraite d'un ancien projet
    deleteTmpPictures();

   QString command="ffmpeg -i ";
   QString fpsOption=" -r ";
   QString outputFormat = " -f image2 ";
   QString output ="%05d.png";

   QString extractCommand= command+movie+fpsOption+frequency+outputFormat+outputBasedir+output;
   qDebug() << extractCommand;
   QByteArray ba = extractCommand.toLocal8Bit();
   const char *c_extractC= ba.data();

   // extraction des images par le system
   system(c_extractC);

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

   //on charge la 1ière images sur drawingArea en la cherchant
   this->ui->widgetRotoscope->setBackground(v_path_backgrounds->at(current_background));

    //incrémentation du nombre de calque pour la première image charger
    this->ui->inputCurrentPicture->setText(QString::number(1));
    this->ui->labelMaxPicture->setText("/"+QString::number(1));
}

void MainWindow::deleteTmpPictures(){

    //suppression du dossier d'image qui peuvent contenir d'anciennes images extraite
    system(QString("rm -Rf "+outputBasedir).toStdString().data());
    //recréation du dossier d'image vide
    system(QString("mkdir "+outputBasedir).toStdString().data());
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
   this->ui->widgetRotoscope->hideBackground(checked);
    background_showed=checked;
}

void MainWindow::on_buttonNewFrame_clicked()
{
    if(nb_frame<v_final_calques->size()){
        nb_frame++;
    }
    //récupération de la dernière version du calques dessiner par l'utilisateur.
            //voir pour le dernier.
    int imageI= this->ui->inputCurrentPicture->text().toInt();
   qDebug() << " newFrame ";
   updateCalque(imageI-1);

    this->ui->widgetRotoscope->setBackground(v_path_backgrounds->at(nb_frame));

    this->ui->inputCurrentPicture->setText(QString::number(nb_frame));
    this->ui->labelMaxPicture->setText("/"+QString::number(nb_frame));
}

void MainWindow::on_actionExport_to_Pictures_triggered()
{   //affichage d'un endroit ou les sauvegarder.
        qDebug()<< "enregistrement";
    for (int x = 0; x < v_final_calques->size(); ++x) {
        v_final_calques->at(x).save("calque"+QString::number(x)+".png");
    }

}



void MainWindow::on_inputCurrentPicture_editingFinished()
{
   if(this->ui->inputCurrentPicture->text().toInt() > 0){
        current_background = this->ui->inputCurrentPicture->text().toInt();

        // un calque par background donc l'index i pour calque = index i pour background
       if(current_background -1 < nb_frame){
         // on retrouve la dernière version du calque correspondant à l'image.
            qDebug()<< "set du calque et du background : " + QString::number(current_background-1);
         this->ui->widgetRotoscope->setCalque(v_final_calques->at(current_background-1));
         if(background_showed){
            this->ui->widgetRotoscope->setBackground(v_path_backgrounds->at(current_background-1));
         }
      }
   }

}


//navigation

void MainWindow::on_buttonNextPicture_clicked()
{

    if(this->ui->inputCurrentPicture->text().toInt()+1 <= nb_frame)
    {


        current_background=this->ui->inputCurrentPicture->text().toInt();

        //mise a jour de l'état du calque avant de passer a l'image suivante
        qDebug() << "next Picture";
        updateCalque(current_background-1);

        this->ui->inputCurrentPicture->setText(QString::number(current_background+1));

        // on met a jour l'image/le calque afficher
        this->ui->widgetRotoscope->setCalque(v_final_calques->at(current_background));
        if(background_showed){
           this->ui->widgetRotoscope->setBackground(v_path_backgrounds->at(current_background));
        }

    }

}

void MainWindow::on_buttonPreviousPicture_clicked()
{
    if(this->ui->inputCurrentPicture->text().toInt() > 1){

        current_background= this->ui->inputCurrentPicture->text().toInt();
        //mise a jour de l'état du calque avant de passer a l'image précédente
        qDebug() << "previous picture";
        updateCalque(current_background-1);


        //maj inteface
        this->ui->inputCurrentPicture->setText(QString::number(current_background-1));

        this->ui->widgetRotoscope->setCalque(v_final_calques->at(current_background-2));
        if(background_showed){
           this->ui->widgetRotoscope->setBackground(v_path_backgrounds->at(current_background-2));
        }
    }


}

void MainWindow::on_buttonLastPicture_clicked()
{

    current_background=this->ui->inputCurrentPicture->text().toInt();

    updateCalque(current_background-1);

    this->ui->widgetRotoscope->setCalque(v_final_calques->last());
    if(background_showed){
       this->ui->widgetRotoscope->setBackground(v_path_backgrounds->last());
    }

    //maj de l'interface
    this->ui->inputCurrentPicture->setText(QString::number(nb_frame));

}



void MainWindow::on_buttonFirstPicture_clicked()
{
    //mise a jour de l'état du calque avant de passer a la dernière image

    current_background=this->ui->inputCurrentPicture->text().toInt();

    this->ui->widgetRotoscope->setCalque(v_final_calques->first());
    if(background_showed){
       this->ui->widgetRotoscope->setBackground(v_path_backgrounds->first());
    }

    //maj de l'interface
    this->ui->inputCurrentPicture->setText(QString::number(1));

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
void MainWindow::updateCalque(int i){
    qDebug() << i;
    v_final_calques->replace(i,this->ui->widgetRotoscope->getLastCalque());


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

