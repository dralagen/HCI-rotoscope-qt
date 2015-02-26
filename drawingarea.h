#ifndef DRAWINGAREA_H
#define DRAWINGAREA_H
#include <iostream>
#include <vector>
#include <QWidget>
#include <QPicture>

class DrawingArea : public QWidget
{
    Q_OBJECT
public:
    explicit DrawingArea(QWidget *parent = 0);
    ~DrawingArea();
    void undo();
    void setPenSize(int size);
    void setTooltype(int tool);
    void setPenColor(QColor c);
    void setBackground(QString path);
    void setCalque(QImage newCalque);
    void setRatio(double r);
    void hideBackground(bool hide);
    QImage getLastCalque();
    void setDrawingCalques(QVector<QImage> v,int nb);
    void setDrawingCalques(QVector<QImage> v);
    void setCurrentCalqueNumber(int nb);

    QSize sizeHint() const;

private:
      QImage *        calque;
      QVector<QImage> v_calques;
      QVector<QImage> v_lastcalqueToDraw;
      QImage *        background;
      QImage *        background_hided;
      QPoint          fPoint;
      QPoint          sPoint;
      QColor          penColor;
      int             penSize;
      int             toolType;
      //nombre de n dernier calque a dessiner.
      int             numberOfCalqueToDraw;
      //numero du calque courant  a partir du quel on dessine les n derniers.
      int             currentCalqueNumber;

      double ratio;

protected:
      void paintEvent(QPaintEvent *event);
      void mouseMoveEvent(QMouseEvent *event);
      void mousePressEvent(QMouseEvent* event);
      void mouseReleaseEvent(QMouseEvent *event);
      void resizeEvent(QResizeEvent *event);

signals:

public slots:

};

#endif // DRAWINGAREA_H

