#ifndef DRAWINGAREA_H
#define DRAWINGAREA_H
#include <iostream>
#include <vector>
#include <QWidget>


class DrawingArea : public QWidget
{
    Q_OBJECT
public:
    explicit DrawingArea(QWidget *parent = 0);
    ~DrawingArea();
private:
      QPoint fPoint;
      QPoint sPoint;
      QColor penColor;
      int    penSize;

protected:
      void paintEvent(QPaintEvent *);
      void mouseMoveEvent(QMouseEvent *event);
      void mousePressEvent(QMouseEvent* event);
      void mouseReleaseEvent(QMouseEvent *event);
signals:

public slots:

};

#endif // DRAWINGAREA_H

