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

   //on charge la 1ière images sur drawingArea en la cherchant
   this->ui->widgetRotoscope->setBackground(v_path_backgrounds->at(current_background));

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


    this->ui->widgetRotoscope->setColor(c);

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



        for (int x = 0; x < v_path_backgrounds->size()-1 ; ++x) {

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
}

void MainWindow::on_buttonNewFrame_clicked()
{
    //récupération de la dernière version du calques dessiner par l'utilisateur.
            //voir pour le dernier.
    v_final_calques->push_back(this->ui->widgetRotoscope->getLastCalque());

    if(current_background < v_path_backgrounds->size()-1){
        current_background++;
      this->ui->widgetRotoscope->setBackground(v_path_backgrounds->at(current_background));
   }
}
