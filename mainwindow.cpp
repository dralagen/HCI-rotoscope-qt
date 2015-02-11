#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "newproject.h"

#include <cstdio>
#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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
        std::cout << newProject.getMovie().toStdString() << std::endl;
    }
    else {
        std::cout << "exit" << std::endl;
    }
}
