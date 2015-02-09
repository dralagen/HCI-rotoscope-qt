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

void NewProject::on_toolButton_clicked()
{
    QFileDialog fileDialog;
    fileDialog.exec();

    QStringList files(fileDialog.selectedFiles());

    QString file(files.first());

    ui->inputMovie->setText(file);
    movie = file;
}
