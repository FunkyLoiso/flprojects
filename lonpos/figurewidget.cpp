#include "figurewidget.h"

FigureWidget::FigureWidget(Figure *FigureL, QWidget *parent) :
    QWidget(parent)
{
    this->setFigure(FigureL);
    this->setSizeCanvasFigure();
}

void FigureWidget::setFigure(Figure* FigureL)
{
    this->paintFigure = FigureL;
}

void FigureWidget::paintForFigure()
{
    for(int i=1; i<this->paintFigure->getSizePointOfFigure();i++)
    {
        this->pointCircle(this->paintFigure->getPoint(i)->getX(),this->paintFigure->getPoint(i)->getY(),this->x,this->y);
    }
}

void FigureWidget::setSizeCanvasFigure()
{
    for(int i=1;i<this->paintFigure->getSizePointOfFigure();i++)
    {
        Point* cur = this->paintFigure->getPoint(i);
        if (this->x<cur->getX()) {this->x=cur->getX();}
        if (this->y<cur->getY()) {this->y=cur->getY();}
    }
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


