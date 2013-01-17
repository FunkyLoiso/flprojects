#ifndef FIGUREPOSITION_H
#define FIGUREPOSITION_H

#include "figure.h"

class FigurePosition : public Figure
{
public:
    FigurePosition();
    void insertPointPosition(int x,int y);              //�������� �����
    Point *getPointPosition(int number);                //�������� �����   (���������� � 1)
private:
    QVector<Point*> figure_matrix_position;
};

#endif // FIGUREPOSITION_H
