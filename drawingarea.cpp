#include "drawingarea.h"
#include <QPainter>
#include <QMouseEvent>
#include <qdebug.h>
#include <qbrush.h>

DrawingArea::DrawingArea(QWidget *parent) :
    QWidget(parent)
{
    penColor= Qt::black;
    penSize = 5;
}

DrawingArea::~DrawingArea(){

}

void DrawingArea::paintEvent(QPaintEvent *e){

   setAttribute(Qt::WA_OpaquePaintEvent);
   QPainter painter(this);
   QPen linepen(penColor); //todo : rendre dynamique en fonction de la couleur
   linepen.setCapStyle(Qt::RoundCap);
   linepen.setWidth(penSize); // todo a rendre dynamique en fonction de la taille du pinceau
   painter.setRenderHint(QPainter::Antialiasing,true);
   painter.setPen(linepen);

   if (fPoint.x() != 0 && fPoint.y()!=0 && sPoint.y()!=0 && sPoint.x()!=0){
       painter.drawLine(fPoint,sPoint);
   }




}

void DrawingArea::mouseMoveEvent(QMouseEvent* event){

            fPoint= sPoint;
            sPoint = event->pos();
            update();

}

void DrawingArea::mousePressEvent(QMouseEvent* event){
         if(event->button()==Qt::LeftButton){
            fPoint= event->pos();
            update();
         }
}

void DrawingArea::mouseReleaseEvent(QMouseEvent * event){
        fPoint.setX(0);
        fPoint.setY(0);
        sPoint.setX(0);
        sPoint.setY(0);

}
