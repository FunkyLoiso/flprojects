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
    void paintFonCircle(Qt::GlobalColor fon, Qt::GlobalColor circle, int x, int y);
    bool paintFonCircle();
	void setcanvasPlace(canvasPlace* canvasLCW);
signals:
    
public slots:

protected:
    void paintEvent(QPaintEvent *);              //виртуальный метод для перерисовки

private:
    int x_weight,y_height;
	canvasPlace* canvasLCW;
	//metod
	void pointCircle(int x, int y, canvasPlace *canvasP);
};

#endif // LONPOSCANVASWIDGET_H
