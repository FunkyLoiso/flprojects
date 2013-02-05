#include "lonposcanvaswidget.h"
#include <qpainter.h>
#include <QtGui>
LonposCanvasWidget::LonposCanvasWidget(QWidget *parent, int x, int y):
    QWidget(parent)
{
    this->x_weight = x;
    this->y_height = y;
}

void LonposCanvasWidget::paintFonCircle(Qt::GlobalColor fon, Qt::GlobalColor circle,int x, int y)
{
    QPainter paint(this);       //объект рисовальщика
    paint.setRenderHint(QPainter::Antialiasing);
    //серый фон
    paint.save();
    paint.setBrush(fon);
    paint.setPen(Qt::NoPen);
    paint.drawRect(0,0,this->width(),this->height());   //квадрат

    //полости для фигур
    paint.restore();
    paint.setPen(Qt::NoPen);
    paint.setBrush(circle);
    int tmp_x = (this->width()/(x+1));   //расстояние между
    int tmp_y = (this->height()/(y+1));
    for(int yi = 1;yi<=y;yi++)
    {
        for(int xi = 1;xi<=x;xi++)
        {
            paint.drawEllipse(QRectF( tmp_x*xi-tmp_x/2,(tmp_y*yi)-tmp_y/2,tmp_x,tmp_y));
        }
    }
}

bool LonposCanvasWidget::paintFonCircle()
{
	if(!this->canvasLCW) {return false;}
    QPainter paint(this);
    paint.setRenderHints(QPainter::Antialiasing);
//    //серый фон
    paint.save();
    paint.setBrush(Qt::white);
    paint.setPen(Qt::NoPen);
    paint.drawRect(0,0,this->width(),this->height());  // квадрат
    paint.restore();

  //  for(int i = 0; i<canvasP.getSizeFigures();i++)
  //  {
  //      //полости фигур
  //      paint.restore();
  //      paint.setPen(Qt::NoPen);
  //      paint.setBrush(canvasP.getFigureP(i+1)->getColor);
		//int tmp_x = (this->width()/(canvasP.getSizeCanvasX()+1));
		//int tmp_y = (this->height()/(canvasP.getSizeCanvasY()+1));
  //      for(int yi = 1;yi <= tmp_y;yi++)
  //      {
  //          for(int xi = 1;xi<=tmp_x;xi++)
  //          {
  //              paint.drawEllipse(QRectF( tmp_x*xi-tmp_x/2,(tmp_y*yi)-tmp_y/2,tmp_x,tmp_y));
  //          }
  //      }
  //  }
}

void LonposCanvasWidget::paintEvent(QPaintEvent *)
{
    //ui.
    //paintFonCircle(Qt::white,Qt::lightGray,x_weight,y_height);
	//paintFonCircle();
	pointCircle(5,5,this->canvasLCW);
}

void LonposCanvasWidget::pointCircle(int x, int y, canvasPlace* canvasP)
{
	int width = (this->width()/(canvasP->getSizeCanvasX()+1));
	int height = (this->height()/(canvasP->getSizeCanvasY()+1));

	QPainter paint(this);
	paint.setPen(Qt::NoPen);
	paint.setBrush(Qt::white);
	paint.drawEllipse(width*x-width/2, height*y-height/2, width, height);
}

void LonposCanvasWidget::setcanvasPlace(canvasPlace* canvasLCW)
{
	this->canvasLCW = canvasLCW;
}
