#include "figurewidget.h"

FigureWidget::FigureWidget(Figure *FigureL, QWidget *parent) :
    QWidget(parent)
{
    this->setFigure(FigureL);
}

void FigureWidget::setFigure(Figure* FigureL)
{
    this->paintFigure = FigureL;
}

void FigureWidget::paintForFigure()
{
//    this->paintFigure->
}

void FigureWidget:: paintEvent(QPaintEvent *)
{

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
