#ifndef CANVASPLACE_H
#define CANVASPLACE_H
//����� ��� �����
#include "figureposition.h"

class canvasPlace
{
public:
    canvasPlace(int x,int y);           //�����������, ������� ������� ������
    void addInvert(int x, int y);       //�������� ����� � ��������������� �������
    void delInvert(int x, int y);       //������� ����� �� ��������������� �������
    bool insertFigure(Figure figure, int x, int y);  //�������� ������ � �����
    void deleteFigure();                             //������� ������ �� ������
    int getSizeCanvasX();                            //�������� ������ ������ X
    int getSizeCanvasY();                            //�������� ������ ������ Y
    int getSizeFigures();                            //�������� ������ ������� �����
	FigurePosition* getFigureP(int num);             //�������� ������ �� ������ �����
private:
	//mb QBitArray ?
    QVector<QVector<bool> > canvas;                 //��������� ������
    QVector<QVector<bool> > invert_canvas;          //��������� �������� �������
    QVector<FigurePosition* >  figures;             //������ ����������� �����
//--------
    int numberShiftCoordinate(int coordinate_one, int coordinate_two);  //���������� ���������� ����� ����� ������������
    Point* shiftCoordinatePoint(Point *center_point, int shift_x, int shift_y);   //���������� ����� �� ���������� ������������
    bool compareFreePosition(FigurePosition *Fpos);///< @todo ��������
};

#endif // CANVASPLACE_H
