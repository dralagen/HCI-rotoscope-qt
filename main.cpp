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
    QFile style("GBstyle.qss");
    style.open(QFile::ReadOnly);
    QString StyleSheet = QLatin1String(style.readAll());
    qApp->setStyleSheet(StyleSheet);
    w.show();
    return a.exec();
}
