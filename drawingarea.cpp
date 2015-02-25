#include "drawingarea.h"
#include <QPainter>
#include <QMouseEvent>
#include <qdebug.h>
#include <qbrush.h>
#include <qimage.h>

#define TOOL_NONE 0
#define TOOL_ERASER 1
#define TOOL_BRUSH 2

//todo :
// faire une liste des calques lors qu'un calque à fini d'être traitrée.
//  export des calques traités => Qimage->save("nom du calques")
//  voir peut être export d'un film ~

DrawingArea::DrawingArea(QWidget *parent) :
    QWidget(parent)
{
    this->resize(774,625);
    penColor= Qt::black;
    penSize = 0;
    toolType=TOOL_NONE;

    calque = new QImage(this->width(),this->height(),QImage::Format_ARGB32_Premultiplied);
    //rendu transparrent du calque
    calque->fill(0);

    background = new QImage(3, 3, QImage::Format_RGB32);
    background->fill(Qt::white);
    //ajustement a la fenêtre de l'image de fond
    background = new QImage(background->scaled(this->size()));

    //image cache de l'image de fond caché
    background_hided = new QImage(3, 3, QImage::Format_RGB32);
    //ajustement a la fenêtre de l'image cache de fond
    background_hided = new QImage(background->scaled(this->size()));

    //initialisation au calque vide.
    v_calques.push_back(*calque);

    numberOfCalqueToDraw=1;

    currentCalqueNumber=1;

}

DrawingArea::~DrawingArea(){
    free(background);
    free(calque);

}

void DrawingArea::paintEvent(QPaintEvent *event){

    setAttribute(Qt::WA_OpaquePaintEvent);
    QPainter c_painter(calque);
    QPen linepen(penColor);
    linepen.setCapStyle(Qt::RoundCap);
    linepen.setWidth(penSize);
    c_painter.setRenderHint(QPainter::Antialiasing,true);
    c_painter.setPen(linepen);
    //painter.save();

    if (toolType == TOOL_ERASER) {
        c_painter.setCompositionMode(QPainter::CompositionMode_Clear);
    }

    //if (fPoint.x() != 0 && fPoint.y()!=0 && sPoint.y()!=0 && sPoint.x()!=0){
    if (fPoint.x() != 0 && fPoint.y()!=0){
       // on dessine la ligne entre 2 point (très proche)
        //c_painter.drawLine(fPoint,sPoint);
        c_painter.drawPoint(fPoint);
    }

    // on dessine sur le widget pour voir l'état courant du dessin
    QPainter widgetPainter(this);
    widgetPainter.drawImage(0,0,*background);


    qDebug() << "-----------------------";
    qDebug() << currentCalqueNumber;
    qDebug() << numberOfCalqueToDraw;
    qDebug() << v_lastcalqueToDraw.size();
    qDebug() << "-----------------------";

    if(numberOfCalqueToDraw >1 && numberOfCalqueToDraw <= v_lastcalqueToDraw.size()){

        if((currentCalqueNumber-numberOfCalqueToDraw)<=0){ // currentC <= nbCalqueToDraw
            numberOfCalqueToDraw=currentCalqueNumber;

            for (int i =1 ; i < numberOfCalqueToDraw; ++i) {
                qDebug() << " currentCalque  inférieur ou égale au nombre de calque a dessiner";
                qDebug()<< i;
                widgetPainter.drawImage(0,0,v_lastcalqueToDraw.at(i-1));
                widgetPainter.drawImage(0,0,* calque);
            }

        }else{
                qDebug() << "passe mais produit rien.";
                qDebug() << "-----------------------";
                qDebug() << currentCalqueNumber;
                qDebug() << numberOfCalqueToDraw;
                qDebug() << "-----------------------";
            for (int i =(currentCalqueNumber-numberOfCalqueToDraw) ; i < currentCalqueNumber; ++i) {
                qDebug() << "currentCalque supérieur au nombre de calque a dessiner";
                qDebug()<< i;
                widgetPainter.drawImage(0,0,v_lastcalqueToDraw.at(i));
                widgetPainter.drawImage(0,0,* calque);
            }
        }

    }else{
         widgetPainter.drawImage(0,0,* calque);

    }

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

}

// en cas de redimensionnement de la fenêtre
void DrawingArea::resizeEvent(QResizeEvent *event){

    background = new QImage(background->scaled(event->size()));
    calque = new QImage(calque->scaled(event->size()));
    update();

}

void DrawingArea::undo(){
    calque = new QImage(v_calques.at(v_calques.size()-2));
    update();
}


void DrawingArea::setTooltype(int tool){
    toolType=tool;
    update();
}

void DrawingArea::setPenSize(int size){
    this->penSize=size;
    update();
}

void DrawingArea::setPenColor(QColor c){
    penColor=c;
    update();
}

void DrawingArea::setCalque(QImage newCalque){
    calque = new QImage(newCalque);
    v_calques.push_back(*calque);
    update();
}

void DrawingArea::setBackground(QString path){
    background= new QImage(path);
    background = new QImage(background->scaled(this->size()));
    update();
}

void DrawingArea::hideBackground(bool hide){

    if(hide){
        background = new QImage(*background_hided);
        background = new QImage(background->scaled(this->size()));

    }
    else
    {
        background_hided = new QImage(*background);
        background= new QImage(3, 3, QImage::Format_RGB32);
        background->fill(Qt::white);
        background= new QImage(background->scaled(this->size()));

    }
     update();
}

QImage DrawingArea::getLastCalque(){

      QImage * lastCalque= new QImage(*calque);
      calque = new QImage(this->width(),this->height(),QImage::Format_ARGB32_Premultiplied);
      calque->fill(0);
      v_calques.clear();
      v_calques.push_back(*calque);

      update();
   return * lastCalque;
}

void DrawingArea::setDrawingCalques(QVector<QImage> v, int nb){
    v_lastcalqueToDraw=v;
    numberOfCalqueToDraw=nb;
    update();
}


void DrawingArea::setCurrentCalqueNumber(int nb){
   currentCalqueNumber=nb;
   update();
}


