#ifndef FIGUREPOSITION_H
#define FIGUREPOSITION_H

#include "figure.h"

class FigurePosition : public Figure
{
public:
    FigurePosition();
    FigurePosition(Figure figure);
    void insertPointPosition(int x,int y); //добавить точку позиции
    void insertPointPosition(Point* pos);  //добавить точку позиции
    Point *getPointPosition(int number);   //получить точку   (нумеруются с 1)
private:
    QVector<Point*> figure_matrix_position;
};

#endif // FIGUREPOSITION_H
