#ifndef LONPOSCANVASWIDGET_H
#define LONPOSCANVASWIDGET_H
//отображения состояния холста
#include <QWidget>
#include <QPainter>
#include "canvasplace.h"

class LonposCanvasWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LonposCanvasWidget( QWidget *parent = 0, int x = 9, int y = 9);
    void setSize(int x, int y);
    void paintFonCircle(int x, int y);
    void paintFonCircle(M2B canvas,M2B invert_canvas);
signals:
    
public slots:

protected:
    void paintEvent(QPaintEvent *);              //виртуальный метод для перерисовки

private:
    int x_weight,y_height;

    
};

#endif // LONPOSCANVASWIDGET_H
