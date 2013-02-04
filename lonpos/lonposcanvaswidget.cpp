#include "lonposcanvaswidget.h"
#include <qpainter.h>
#include <QtGui>
LonposCanvasWidget::LonposCanvasWidget(QWidget *parent, int x, int y):
    QWidget(parent)
{
    this->x_weight = x;
    this->y_height = y;
}

bool LonposCanvasWidget::setSize(int x, int y)
{
    this->x_weight = x;
    this->y_height = y;
}

void LonposCanvasWidget::paintEvent(QPaintEvent *)
{
    QPainter paint(this);       //объект рисовальщика
    paint.setRenderHint(QPainter::Antialiasing);
    paint.save();

    //серый фон
//    paint.restore();
    paint.setBrush(Qt::gray);
    paint.setPen(Qt::NoPen);
    paint.drawRect(0,0,this->width(),this->height());

    //полости для фигур
    paint.restore();
    paint.setPen(Qt::NoPen);
    paint.setBrush(Qt::lightGray);
    int tmp_x = (this->width()/(this->x_weight+1));
    int tmp_y = (this->height()/(this->y_height+1));
    for(int yi = 1;yi<=this->y_height;yi++)
    {
        for(int xi = 1;xi<=this->x_weight;xi++)
        {
            paint.drawEllipse(QRectF( tmp_x*xi-tmp_x/2,(tmp_y*yi)-tmp_y/2,tmp_x,tmp_y));
        }
    }



}
