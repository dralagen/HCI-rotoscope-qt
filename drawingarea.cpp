#include "drawingarea.h"
#include <QPainter>
#include <QMouseEvent>
#include <qdebug.h>
#include <qbrush.h>
#include <qimage.h>

//todo : voir le bugue de mémoire corrompu avec le qvector<qimage>
// =>  qui n'apparait plus après quelques lancements

DrawingArea::DrawingArea(QWidget *parent) :
    QWidget(parent)
{
    this->resize(774,625);
    penColor= Qt::blue;
    penSize = 5;

    calque = new QImage(this->width(),this->height(),QImage::Format_ARGB32_Premultiplied);
    //rendu transparrent du calque
    calque->fill(0);

    background = new QImage("/home/david/Documents/M1ALMA/IHM/projet/images/dks.jpg");
    //ajustement a la fenêtre de l'image de fond
    background = new QImage(background->scaled(this->size()));
    //ignitialisation au calque vide.
    v_calques.push_back(*calque);

}

DrawingArea::~DrawingArea(){

}

void DrawingArea::paintEvent(QPaintEvent *event){

   setAttribute(Qt::WA_OpaquePaintEvent);
   QPainter painter(calque);
   QPen linepen(penColor);
   linepen.setCapStyle(Qt::RoundCap);
   linepen.setWidth(penSize);
   painter.setRenderHint(QPainter::Antialiasing,true);
   painter.setPen(linepen);


   if (fPoint.x() != 0 && fPoint.y()!=0 && sPoint.y()!=0 && sPoint.x()!=0){
      // on dessine la ligne entre 2 point (très proche)
       painter.drawLine(fPoint,sPoint);
   }
   painter.save();

   // on dessine sur le widget pour voir l'état courant du dessin
   QPainter widgetPainter(this);
   widgetPainter.drawImage(0,0,*background);
   widgetPainter.drawImage(0,0,*calque);

}
//mouvement de la sourie
void DrawingArea::mouseMoveEvent(QMouseEvent* event){

            fPoint= sPoint;
            sPoint = event->pos();
            update();

}

//clique
void DrawingArea::mousePressEvent(QMouseEvent* event){

        if(event->button()==Qt::LeftButton){
            fPoint= event->pos();
         }
        update();
}

//au clique relevé
void DrawingArea::mouseReleaseEvent(QMouseEvent * event){

        //reset des positions du curseur enregistrer pour éviter de repartir du dernier point.
        fPoint.setX(0);
        fPoint.setY(0);
        sPoint.setX(0);
        sPoint.setY(0);
        v_calques.push_back(*calque);
        //calque->save("image2.bmp"); //a effectué en boucle sur le claqueList lors de l'export de chaque calque.

}

// en cas de redimensionnement de la fenêtre
void DrawingArea::resizeEvent(QResizeEvent *event){

    background = new QImage(background->scaled(event->size()));
    calque= new QImage(calque->scaled(event->size()));
}

void DrawingArea::undo(){
    calque = new QImage(v_calques.at(v_calques.size()-2));
    update();
}

void DrawingArea::setPenSize(int size){
        this->penSize=size;
        update();
}
