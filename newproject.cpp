#include "newproject.h"
#include "ui_newproject.h"

#include <QFileDialog>

#include <iostream>

NewProject::NewProject(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewProject)
{
    ui->setupUi(this);
}

NewProject::~NewProject()
{
    delete ui;
}

QString NewProject::getMovie() {
    return movie;
}

QString NewProject::getFrequency(){
    return frequency;
}

void NewProject::on_toolButton_clicked()
{
    QFileDialog fileDialog;
    fileDialog.exec();

    if (fileDialog.result() == QDialog::Accepted) {
        QStringList files(fileDialog.selectedFiles());

        QString file(files.first());

        ui->inputMovie->setText(file);
        movie = file;
    }

}

void NewProject::on_buttonBox_accepted()
{
  if(frequency==""){
      frequency="4";
  }

}

void NewProject::on_freq4_clicked()
{
    frequency=ui->freq4->text();
}


void NewProject::on_freq6_clicked()
{
     frequency=ui->freq6->text();
}

void NewProject::on_freq8_clicked()
{
    frequency=ui->freq8->text();
}



void NewProject::on_freq12_clicked()
{
    frequency=ui->freq12->text();
}

void NewProject::on_freq24_clicked()
{
    frequency=ui->freq24->text();
}
