#-------------------------------------------------
#
# Project created by QtCreator 2015-02-09T11:08:10
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QMAKE_CXXFLAGS += -std=c++11

TARGET = rotoscope
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
        newproject.cpp \
    drawingarea.cpp

HEADERS  += mainwindow.h \
            newproject.h \
    drawingarea.h

FORMS    += mainwindow.ui \
            newproject.ui

style.path  = "$${OUT_PWD}"
style.files = GBstyle.qss

images.path    = "$${OUT_PWD}/icones"
images.files   += icones/first.jpg
images.files   += icones/gome.png
images.files   += icones/last.jpg
images.files   += icones/next.jpg
images.files   += icones/pinceau.png
images.files   += icones/play.jpg
images.files   += icones/previous.jpg

INSTALLS += images style
