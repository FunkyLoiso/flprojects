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
    //test
    void addCellCanvas(int x, int y);   //�������� ����� � �����
    void delCellCanvas(int x, int y);   //������� ����� �� ������
    //
    bool insertFigure(Figure figure, int x, int y);  //�������� ������ � �����
    void deleteFigure();                             //������� ������ �� ������
    //
    bool getStateInvertCanvas(int x, int y);//c 0    //�������� ��������� ������ ���������������� ������
    bool getStateCanvas(int x, int y);//c 0          //�������� ��������� ������ ������
    //
    int getSizeCanvasX();                            //�������� ������ ������ X
    int getSizeCanvasY();                            //�������� ������ ������ Y
    //
    int getSizeFigures();                            //�������� ������ ������� ����������� �����
    FigurePosition* getFigureP(int num);             //�������� ������ �� ������ ����������� �����
    //
//    int getSizeExistFigures();                       //�������� ������ ������� ������������ �����
//    Figure* getExistFigures(int num);               //�������� ������ �� ������ ������������ �����
private:
	//mb QBitArray ?
    QVector<QVector<bool> > canvas;                 //��������� ������
    QVector<QVector<bool> > invert_canvas;          //��������� �������� �������
    QVector<FigurePosition* >  figures;             //������ ����������� �����
//    QVector<Figure* > exist_figures;               //������������ ������
//--------
    int numberShiftCoordinate(int coordinate_one, int coordinate_two);  //���������� ���������� ����� ����� ������������
    Point* shiftCoordinatePoint(Point *center_point, int shift_x, int shift_y);   //���������� ����� �� ���������� ������������
    bool compareFreePosition(FigurePosition *Fpos);///< @todo ��������
};

#endif // CANVASPLACE_H
