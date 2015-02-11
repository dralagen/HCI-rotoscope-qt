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
private:
      QImage * calque;
      QVector<QImage> v_calques;
      QImage * background;
      QPoint fPoint;
      QPoint sPoint;
      QColor penColor;
      int    penSize;


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

