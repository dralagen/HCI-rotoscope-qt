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
    void hideBackground(bool hide);
    QImage getLastCalque();

private:
      QImage *        calque;
      QVector<QImage> v_calques;
      QImage *        background;
      QImage *        background_hided;
      QPoint          fPoint;
      QPoint          sPoint;
      QColor          penColor;
      int             penSize;
      int             toolType;

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

