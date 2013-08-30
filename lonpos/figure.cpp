#include "figure.h"

Point::Point()
{
}

Point::Point(int x,int y)
{
    Point::setX(x);
    Point::setY(y);
}

int Point::getX()
{
    return this->x;
}

int Point::getY()
{
    return this->y;
}

void Point::setX(int coordinate)
{
    this->x=coordinate;
}

void Point::setY(int coordinate)
{
    this->y=coordinate;
}

Figure::Figure()
{
    this->degree=0;
}

void Figure::insertPoint(int x, int y)
{
    Point* tmp = new Point(x,y);
    Figure::figure_matrix.append(tmp);
}

void Figure::insertPoint(Point *pos)
{
    Figure::figure_matrix.append(pos);
}

Point* Figure::getPoint(int number)
{
    if (number>=0)
    {
        return figure_matrix[number-1];
    }
    return NULL;
}

QVector<Point *> Figure::getFigureMatrix()
{
    return figure_matrix;
}

Point* Figure::turnPoint(Point *point,int degree)
{
    int tmp_x,tmp_y;
    tmp_x=point->getX()*qCos(degree*M_PI/180)-point->getY()*qSin(degree*M_PI/180);
    tmp_y=point->getX()*qSin(degree*M_PI/180)+point->getY()*qCos(degree*M_PI/180);
    point->setX(tmp_x);
    point->setY(tmp_y);
    return point;
}

void Figure::turnFigureMatrix(int degree)
{
    for(int i=0;i<figure_matrix.size();i++)
    {
        figure_matrix[i]=turnPoint(figure_matrix[i],degree);
    }
    this->degree=degree;
}

void Figure::mirrorFigureMatrix()
{
    for(int i=figure_matrix.size();i>0;i--)
    {
        figure_matrix[i-1]->setX(figure_matrix[i-1]->getX()*(-1));
        figure_matrix[i-1]->setY(figure_matrix[i-1]->getY()*(-1));
    }
    //чтобы градусы не были меньше 0
    if(this->degree>180) {this->degree-=180;}
    if(this->degree<180) {this->degree+=180;}
}

int  Figure::getSizePointOfFigure()
{
    return figure_matrix.size();
}

int Figure::getDegree()
{
    return this->degree;
}

QColor Figure::getColor()
{
    return this->colorF;
}

void Figure::setColor(QColor incolorF)
{
    this->colorF = incolorF;
}

void Figure::clearPoints()
{
    figure_matrix.clear();
}




