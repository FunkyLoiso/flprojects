#ifndef FIGURE_H
#define FIGURE_H

//#include <QVector2D>
#include <qvector.h>
#include <qmath.h>

class Point
{
public:
    Point(int x, int y);
    int getX();
    int getY();
    void setX(int coordinate);
    void setY(int coordinate);
private:
    int x,y;
};

//реализация фигуры
class Figure
{
public:
    Figure();                           //конструктор
    void insertPoint(int x,int y);      //добавить точку
    Point *getPoint(int number);        //получить точку   (нумеруются с 1)
    QVector<Point *> getFigureMatrix(); //получить матрицу фигуры
    void turnFigureMatrix(int degree);  //повернуть все точки на угол(градусы)
    void mirrorFigureMatrix();          //отзеркалировать фигуру
private:
    QVector<Point*> figure_matrix;
    int degree;
//-------
    Point *turnPoint(Point *point, int degree); //повернуть точку на угол(градусы)
};
#endif // FIGURE_H
