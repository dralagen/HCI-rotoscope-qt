#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "newproject.h"
#include <cstdio>
#include <iostream>
#include <QDebug>
#include <QColorDialog>

#define TOOL_ERASER 1
#define TOOL_BRUSH 2


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    v_color = new QVector<QColor>();

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

   QString command="ffmpeg -i ";
   QString fpsOption=" -r ";
   QString output =" /tmp/image-%05d.png";

   QString extractCommand= command+movie+fpsOption+frequency+output;
   qDebug() << extractCommand;
   QByteArray ba = extractCommand.toLocal8Bit();
   const char *c_extractC= ba.data();

   system(c_extractC);

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
