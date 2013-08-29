#ifndef FIGURE_H
#define FIGURE_H
//������ + �����
#include <qvector.h>
#include <qmath.h>
#include <qcolor.h>

//��� ������ QPoint ..?
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

//���������� ������
class Figure
{
public:
    Figure();                           //�����������
    void insertPoint(int x,int y);      //�������� �����
    void insertPoint(Point* pos);       //�������� �����
    Point *getPoint(int number);        //�������� �����   (���������� � 1)
    QVector<Point *> getFigureMatrix(); //�������� ������� ������
    void turnFigureMatrix(int degree);  //��������� ��� ����� �� ����(�������)
    void mirrorFigureMatrix();          //��������������� ������
    int getSizePointOfFigure();         //���������� ����� � ������
    int getDegree();                    //������� ���� ��������
    QColor getColor();                  //�������� ���� ������
    void   setColor(QColor incolorF);   //���������� ���� ������
    void clearPoints();                 //������� ��� �����
protected:
    QVector<Point*> figure_matrix;
    QColor colorF;                      //����� ���� ������
    int degree;                         //��� �������� ����� 0
//-------
    Point *turnPoint(Point *point, int degree); //��������� ����� �� ����(�������)
};
#endif // FIGURE_H
