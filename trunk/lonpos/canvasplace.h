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
    QVector<QVector<bool> > canvas;                 //двумерный булеан
    QVector<QVector<bool> > invert_canvas;          //двумерная инверсия булеана
    QVector<FigurePosition* >  figures;             //массив вставленныйх фигур
//--------
    int numberShiftCoordinate(int coordinate_one, int coordinate_two);  //возвращает расстояние между двумя координатами
    Point* shiftCoordinatePoint(Point *center_point, int shift_x, int shift_y);   //возвращает точку со смещенными координатами
    bool compareFreePosition(FigurePosition *Fpos);
};

#endif // CANVASPLACE_H
