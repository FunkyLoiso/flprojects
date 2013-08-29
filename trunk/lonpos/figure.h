#ifndef FIGURE_H
#define FIGURE_H
//фигура + точки
#include <qvector.h>
#include <qmath.h>
#include <qcolor.h>

//как насчёт QPoint ..?
class Point
{
public:
    Point();
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
    void insertPoint(Point* pos);       //добавить точку
    Point *getPoint(int number);        //получить точку   (нумеруются с 1)
    QVector<Point *> getFigureMatrix(); //получить матрицу фигуры
    void turnFigureMatrix(int degree);  //повернуть все точки на угол(градусы)
    void mirrorFigureMatrix();          //отзеркалировать фигуру
    int getSizePointOfFigure();         //количество точек в фигуре
    int getDegree();                    //вернуть угол поворота
    QColor getColor();                  //получить цвет фигуры
    void   setColor(QColor incolorF);   //установить цвет фигуры
    void clearPoints();                 //удалить все точки
protected:
    QVector<Point*> figure_matrix;
    QColor colorF;                      //общий цвет фигуры
    int degree;                         //при создании равен 0
//-------
    Point *turnPoint(Point *point, int degree); //повернуть точку на угол(градусы)
};
#endif // FIGURE_H
