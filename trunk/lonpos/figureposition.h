#ifndef FIGUREPOSITION_H
#define FIGUREPOSITION_H

#include "figure.h"

class FigurePosition : public Figure
{
public:
    FigurePosition();
    void insertPointPosition(int x,int y);              //добавить точку
    Point *getPointPosition(int number);                //получить точку   (нумеруются с 1)
private:
    QVector<Point*> figure_matrix_position;
};

#endif // FIGUREPOSITION_H
