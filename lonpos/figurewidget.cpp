#include "figurewidget.h"

FigureWidget::FigureWidget(Figure *FigureL, QWidget *parent) :
    QWidget(parent)
{
    this->setFigure(FigureL);
//    this->x=5;
//    this->y=5;
    this->setSizeCanvasFigure();
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
    this->x=abs(max_x-min_y)+1;  //������ ��� (-1,2) ���������� ����� ���� 3, � ����� 4
    this->y=abs(max_y-min_y)+1;
}

void FigureWidget:: paintEvent(QPaintEvent *)
{
    QPainter paint(this);
    paint.setRenderHints(QPainter::Antialiasing);
    paint.setPen(Qt::NoPen);
    paint.setBrush(Qt::green);
    paint.drawRect(0,0,this->width(),this->height());
    this->paintForFigure();
}

void FigureWidget::pointCircle(int x, int y, int numberX, int numberY, QColor color/*=QColor(160,152,152)*/)
{
    //������ ����
//    if ()
    int width = (this->width()/(numberX));
    int height = (this->height()/(numberY));
	int size = qMin(width, height);

//    int size_long_X=numberX*size;
//    int size_long_Y=numberY*size;
//    int borderX=this->width()-size_long_X;
//    int borderY=this->height()-size_long_Y;

    QPainter paint(this);
    paint.setRenderHints(QPainter::Antialiasing);
    paint.setPen(Qt::NoPen);
    paint.setBrush(color);

    paint.drawEllipse(size*(x)/*-size/2*/, size*(y)/*-size/2*/, size, size);
//�������� � ������ �� x - �������� ������ ����� �������� ������ ������
//    int centerShiftX = this->width()/2 - numberX*size/2;
//    int centerShiftY = this->height()/2 - numberY*size/2;

//    paint.drawEllipse(size*(x+1) + centerShiftX, size*(y+1) + centerShiftY, size, size);
}


