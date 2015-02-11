#include "mainwindow.h"
#include <QApplication>
#include <QtGui>
#include <QtWidgets>
#include <QToolBar>
#include <QMenuBar>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    //w.init();
    w.show();
    return a.exec();
}
