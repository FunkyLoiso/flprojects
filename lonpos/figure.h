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

//���������� ������
class Figure
{
public:
    Figure();                           //�����������
    void insertPoint(int x,int y);      //�������� �����
    Point *getPoint(int number);        //�������� �����   (���������� � 1)
    QVector<Point *> getFigureMatrix(); //�������� ������� ������
    void turnFigureMatrix(int degree);  //��������� ��� ����� �� ����(�������)
    void mirrorFigureMatrix();          //��������������� ������
private:
    QVector<Point*> figure_matrix;
    int degree;
//-------
    Point *turnPoint(Point *point, int degree); //��������� ����� �� ����(�������)
};
#endif // FIGURE_H
