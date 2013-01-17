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
    if (number>0)
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
    tmp_x=point->getX()*qCos(degree)-point->getY()*qSin(degree);
    tmp_y=point->getX()*qSin(degree)+point->getY()*qCos(degree);
    point->setX(tmp_x);
    point->setY(tmp_y);
    return point;
}

void Figure::turnFigureMatrix(int degree)
{
    for(int i=figure_matrix.size();i>0;i--)
    {
        figure_matrix[i-1]=turnPoint(figure_matrix[i-1],degree);
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
}

int  Figure::getSizePointOfFigure()
{
    return figure_matrix.size();
}




