#include "lonposcanvaswidget.h"
#include <qpainter.h>
#include <QtGui>
LonposCanvasWidget::LonposCanvasWidget(QWidget *parent):
    QWidget(parent)
{
}

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

    int size = qMin(width,height);

	QPainter paint(this);
    paint.setRenderHints(QPainter::Antialiasing);
	paint.setPen(Qt::NoPen);
    paint.setBrush(color);
    paint.drawEllipse(size*(x+1)-size/2, size*(y+1)-size/2, size, size);
//    paint.drawEllipse(width*(x+1)-width/2, height*(y+1)-height/2, width, height);
}

//присвоить указатель на холст
void LonposCanvasWidget::setcanvasPlace(canvasPlace* canvasLCW)
{
	this->canvasLCW = canvasLCW;
}
