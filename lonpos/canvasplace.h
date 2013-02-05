#ifndef CANVASPLACE_H
#define CANVASPLACE_H
//����� ��� �����
#include "figureposition.h"

typedef QVector<QVector<bool> >  M2B;

class canvasPlace
{
public:
    canvasPlace(int x,int y);           //�����������, ������� ������� ������
    void addInvert(int x, int y);       //�������� ����� � ��������������� �������
    void delInvert(int x, int y);       //������� ����� �� ��������������� �������
    bool insertFigure(Figure figure, int x, int y);  //�������� ������ � �����
    void deleteFigure();                             //������� ������ �� ������
    QVector<QVector<bool> > getCanvas();
    QVector<QVector<bool> > getInvertCanvas();
private:
    QVector<QVector<bool> > canvas;                 //��������� ������
    QVector<QVector<bool> > invert_canvas;          //��������� �������� �������
    QVector<FigurePosition* >  figures;             //������ ������������ �����
//--------
    int numberShiftCoordinate(int coordinate_one, int coordinate_two);  //���������� ���������� ����� ����� ������������
    Point* shiftCoordinatePoint(Point *center_point, int shift_x, int shift_y);   //���������� ����� �� ���������� ������������
    bool compareFreePosition(FigurePosition *Fpos);
};

#endif // CANVASPLACE_H
