#include "lonposcanvaswidget.h"
#include <qpainter.h>
#include <QtGui>
LonposCanvasWidget::LonposCanvasWidget(QWidget *parent):
    QWidget(parent)
{
//    this->x_weight = this->canvasLCW->getSizeCanvasX();
//    this->y_height = this->canvasLCW->getSizeCanvasY();

//    this->x_weight = x;
//    this->y_height = y;
}

//void LonposCanvasWidget::paintFonCircle(Qt::GlobalColor fon, Qt::GlobalColor circle,int x, int y)
//{
//    QPainter paint(this);       //объект рисовальщика
//    paint.setRenderHint(QPainter::Antialiasing);
//    //серый фон
//    paint.save();
//    paint.setBrush(fon);
//    paint.setPen(Qt::NoPen);
//    paint.drawRect(0,0,this->width(),this->height());   //квадрат

//    //полости для фигур
//    paint.restore();
//    paint.setPen(Qt::NoPen);
//    paint.setBrush(circle);
//    int tmp_x = (this->width()/(x+1));   //расстояние между центрами кругов
//    int tmp_y = (this->height()/(y+1));  //up
//    for(int yi = 1;yi<=y;yi++)
//    {
//        for(int xi = 1;xi<=x;xi++)
//        {
//            paint.drawEllipse(QRectF( tmp_x*xi-tmp_x/2,(tmp_y*yi)-tmp_y/2,tmp_x,tmp_y));
//        }
//    }
//}

bool LonposCanvasWidget::paintFonCircle()
{
	if(!this->canvasLCW) {return false;}
    QPainter paint(this);
    paint.setRenderHints(QPainter::Antialiasing);
    //серый фон
    paint.save();
    paint.setBrush(Qt::white);
    paint.setPen(Qt::NoPen);
    paint.drawRect(0,0,this->width(),this->height());  // квадрат
    paint.restore();

    //полости фигур
    paint.restore();
    paint.setPen(Qt::NoPen);
    int canvasSizeX = this->canvasLCW->getSizeCanvasX();
    int canvasSizeY = this->canvasLCW->getSizeCanvasY();

    //отрисовка того что вставленно в холст
    for(int yi = 0;yi<canvasSizeY;yi++)
    {
       for(int xi = 0;xi<canvasSizeX;xi++)
       {
           if(canvasLCW->getStateCanvas(xi,yi))
           {
               pointCircle(xi,yi,canvasSizeX,canvasSizeY,Qt::blue);
           }
       }
    }
    //отрисовка инвертированного холста
    for(int yi = 0;yi<canvasSizeY;yi++)
    {
       for(int xi = 0;xi<canvasSizeX;xi++)
       {
           if(canvasLCW->getStateInvertCanvas(xi,yi))
           {
               pointCircle(xi,yi,canvasSizeX,canvasSizeY,Qt::red);
           }
       }
    }
    return true;
}

void LonposCanvasWidget::paintEvent(QPaintEvent *)
{
    paintFonCircle();
}

//нарисовать заданный круг на холсте
void LonposCanvasWidget::pointCircle(int x, int y, int numberX, int numberY,QColor color)
{
    int width = (this->width()/(numberX+1));
    int height = (this->height()/(numberY+1));

	QPainter paint(this);
    paint.setRenderHints(QPainter::Antialiasing);
	paint.setPen(Qt::NoPen);
    paint.setBrush(color);
    paint.drawEllipse(width*(x+1)-width/2, height*(y+1)-height/2, width, height);
}

//присвоить указатель на холст
void LonposCanvasWidget::setcanvasPlace(canvasPlace* canvasLCW)
{
	this->canvasLCW = canvasLCW;
}
