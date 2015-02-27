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
    penColor= Qt::white;
    penSize = 0;
    ratio = 0.0;
    toolType=TOOL_NONE;

    calque = new QImage(this->width(),this->height(),QImage::Format_ARGB32_Premultiplied);
    //rendu transparrent du calque
    calque->fill(0);

    background = new QImage(3, 3, QImage::Format_RGB32);
    background->fill(Qt::gray);
    //ajustement a la fenêtre de l'image de fond
    background = new QImage(background->scaled(this->size()));

    background_hided = false;

    //initialisation au calque vide.
    v_calques.push_back(*calque);

    numberOfCalqueToDraw=0;
    freqOfCalqueToDraw=1;

    currentCalqueNumber=0;

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

    if (fPoint.x() != 0 && fPoint.y() != 0){
        if (sPoint.x() != 0 && sPoint.y() != 0) {
            // on dessine la ligne entre 2 point (très proche)
            c_painter.drawLine(fPoint,sPoint);
        } else {
            c_painter.drawPoint(fPoint);
        }

    }

    // on dessine sur le widget pour voir l'état courant du dessin
    QPainter widgetPainter(this);

    widgetPainter.fillRect(0,0, this->width(), this->height(), Qt::gray);

    if (!background_hided) {
        widgetPainter.drawImage(0,0,*background);
    }
    else {
        widgetPainter.fillRect(0,0,background->width(), background->height(), Qt::black);
    }


    qDebug() << "-----------------------";
    qDebug() << " currentCalqueNumber       : " << currentCalqueNumber;
    qDebug() << " numberOfCalqueToDraw      : " << numberOfCalqueToDraw;
    qDebug() << " v_lastcalqueToDraw.size() : " << v_lastcalqueToDraw.size();
    qDebug() << "-----------------------";

    const int firstIndice = std::max((currentCalqueNumber - numberOfCalqueToDraw*freqOfCalqueToDraw), 0);

    for (int i = currentCalqueNumber-freqOfCalqueToDraw; i >= firstIndice; i-=freqOfCalqueToDraw) {

        widgetPainter.setOpacity((0.4 / (double)numberOfCalqueToDraw*freqOfCalqueToDraw) * (double)(i-firstIndice) + 0.4);
        widgetPainter.drawImage(0,0,v_lastcalqueToDraw.at(i));

    }
    widgetPainter.setOpacity(1);
    widgetPainter.drawImage(0,0,* calque);



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

    QSize newSize(sizeHint());
    background = new QImage(background->scaled(newSize));
    calque = new QImage(calque->scaled(newSize));

    updateGeometry();

}

QSize DrawingArea::sizeHint() const {
    QSize s = size();

    if (ratio != 0.0) {
        if (s.height() < ratio * s.width()) {
            s.setWidth(1/ratio * s.height());
        } else {
            s.setHeight(ratio * s.width());
        }
    }

    return s;
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
    calque = new QImage(newCalque.scaled(sizeHint()));
    v_calques.push_back(*calque);
    update();
}

void DrawingArea::setRatio(double r)
{
    ratio = (r>=0)?r:0;
}

void DrawingArea::setBackground(QString path){
    background= new QImage(path);
    background = new QImage(background->scaled(sizeHint()));
    update();
}

void DrawingArea::hideBackground(bool hide){
    background_hided = hide;
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

void DrawingArea::setDrawingCalques(QVector<QImage> v){
    setDrawingCalques(v, numberOfCalqueToDraw);
}

void DrawingArea::setFreqDrawindCalques(int f)
{
    freqOfCalqueToDraw=std::max(f,1);
    update();
}

int DrawingArea::getFreqDrawingCalques()
{
    return freqOfCalqueToDraw;
}


void DrawingArea::setCurrentCalqueNumber(int nb){
   currentCalqueNumber=nb;
   update();
}


