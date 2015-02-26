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
    fileDialog.setWindowTitle("Open Video");
    fileDialog.setFileMode(QFileDialog::ExistingFile);
    fileDialog.setNameFilter("AllVideo (*.3g2 *.3gp *.asf *.avi *.flv *.m4v *.mkv *.mov *.qt *.mp4 *.m4p *.mpg *.mp2 *.mpeg *.mpe *.mpv *.m2v *.ogv *.ogg *.webm *.wmv *.yuv);;"
                      "Cell Phone Video (*.3g2 *.3gp);;"
                      "AVI (*.avi);;"
                      "Flash Video (*.flv);;"
                      "Apple Format (*.m4v);;"
                      "Matroska (*.mkv);;"
                      "QuickTime (*.mov *.qt);;"
                      "MPEG-4 (*.mp4 *.m4p *.m4v);;"
                      "MPEG-1 (*.mpg *.mp2 *.mpeg *.mpe *.mpv);;"
                      "MPEG-2 (*.mpg *.mpeg *.m2v);;"
                      "Ogg Video (*.ogv *.ogg);;"
                      "WebM (*.webm);;"
                      "Windows Media Video (*.wmv);;"
                      "Raw video format (*.yuv)");
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
