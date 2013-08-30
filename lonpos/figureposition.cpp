#include "figureposition.h"

FigurePosition::FigurePosition()
{
}

FigurePosition::FigurePosition(Figure* figure)
{
    this->figure_matrix = figure->getFigureMatrix();
    this->degree = figure->getDegree();
}

void FigurePosition::insertPointPosition(int x, int y)
{
    Point *tmp = new Point(x,y);
    FigurePosition::figure_matrix_position.append(tmp);
}

void FigurePosition::insertPointPosition(Point *pos)
{
    FigurePosition::figure_matrix_position.append(pos);
}

Point *FigurePosition::getPointPosition(int number)
{
    if (number>0)
    {
        return figure_matrix_position[number-1];
    }
    return NULL;
}

