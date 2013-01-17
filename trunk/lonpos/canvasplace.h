#ifndef CANVASPLACE_H
#define CANVASPLACE_H

#include "figureposition.h"

class canvasPlace
{
public:
    canvasPlace(int x,int y);           //конструктор, размеры матрицы холста
    void addInvert(int x, int y);       //добавить точку в инвертированную матрицу
    void delInvert(int x, int y);       //удалить точку из инвертированной матрицы
    bool insertFigure(Figure figure, int x, int y);  //вставить фигуру в холст
    void deleteFigure();                             //удалить фигуру из холста
private:
    QVector<QVector<bool> > canvas;
    QVector<QVector<bool> > invert_canvas;
    QVector<FigurePosition* >  figures;
//--------
    int numberShiftCoordinate(int coordinate_one, int coordinate_two);
    int shiftCoordinate(Point center_point, int x, int y);
};

#endif // CANVASPLACE_H
