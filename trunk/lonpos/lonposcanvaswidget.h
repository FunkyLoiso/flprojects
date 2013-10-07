#ifndef LONPOSCANVASWIDGET_H
#define LONPOSCANVASWIDGET_H
//отображения состояния холста

#if QT_VERSION < 0x050000
	#include <QWidget>
#else
	#include <QtWidgets/qwidget.h>
#endif

#include <QPainter>
#include "canvasplace.h"

class LonposCanvasWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LonposCanvasWidget( QWidget *parent = 0/*, int x = 9, int y = 9*/);
//    void setSize(int x, int y);
    bool paintFonCircle();                        //отрисовка  того что вставленно в холст и инверсного холста
    void setcanvasPlace(canvasPlace* canvasLCW);  //присвоить указатель на холст
signals:
    
public slots:

protected:
    virtual void paintEvent(QPaintEvent *);              //виртуальный метод для перерисовки (ЭТА ХРЕНЬ РИСУЕТ)
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *);

private:
    QPoint  test;

    QColor colorC;
    canvasPlace* canvasLCW;                      //указатель на созданный холст
    //нарисовать кружок в точке x и y, начинается с 0
    void pointCircle(int x, int y, int numberX, int numberY, QColor color=QColor(160,152,152));
};

#endif // LONPOSCANVASWIDGET_H
