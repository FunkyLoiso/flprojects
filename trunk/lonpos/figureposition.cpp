#include "figureposition.h"

FigurePosition::FigurePosition()
{
}

void FigurePosition::insertPointPosition(int x, int y)
{
    Point *tmp = new Point(x,y);
    FigurePosition::figure_matrix_position.append(tmp);
}

Point *FigurePosition::getPointPosition(int number)
{
    if (number>0)
    {
        return figure_matrix_position[number-1];
    }
    return NULL;
}

