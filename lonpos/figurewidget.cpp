#include "figurewidget.h"

FigureWidget::FigureWidget(Figure *FigureL, QWidget *parent) :
    QWidget(parent)
{
    this->setFigure(FigureL);
    this->x=5;
    this->y=5;
//    this->setSizeCanvasFigure();
}

void FigureWidget::setFigure(Figure* FigureL)
{
    this->paintFigure = FigureL;
}

void FigureWidget::paintForFigure()
{
    for(int i=1; i<=this->paintFigure->getSizePointOfFigure();i++)
    {
        this->pointCircle(this->paintFigure->getPoint(i)->getX(),this->paintFigure->getPoint(i)->getY(),this->x,this->y);
    }
}

void FigureWidget::setSizeCanvasFigure()
{
    Point* cur;
    int min_x,min_y,max_x,max_y;

    max_x=this->paintFigure->getPoint(1)->getX();
    min_x=this->paintFigure->getPoint(1)->getX();
    max_y=this->paintFigure->getPoint(1)->getY();
    min_y=this->paintFigure->getPoint(1)->getY();
    for(int i=1;i<=this->paintFigure->getSizePointOfFigure();i++)
    {
        cur = this->paintFigure->getPoint(i);
        if (max_x<cur->getX()) {max_x=cur->getX();}
        if (min_x>cur->getX()) {min_x=cur->getX();}
        if (max_y<cur->getY()) {max_y=cur->getY();}
        if (min_y>cur->getY()) {min_y=cur->getY();}
     }
    this->x=abs(min_x-max_x);
    this->y=abs(min_y-max_y);
}

void FigureWidget:: paintEvent(QPaintEvent *)
{
    this->paintForFigure();
}

void FigureWidget::pointCircle(int x, int y, int numberX, int numberY, QColor color)
{
    int width = (this->width()/(numberX+1));
    int height = (this->height()/(numberY+1));

    QPainter paint(this);
    paint.setRenderHints(QPainter::Antialiasing);
    paint.setPen(Qt::NoPen);
    paint.setBrush(color);
    paint.drawEllipse(width*(x+1)-width/2, height*(y+1)-height/2, width, height);
}

