#ifndef CANVASPLACE_H
#define CANVASPLACE_H
//холст дл€ фигур
#include "figureposition.h"

class canvasPlace
{
public:
    canvasPlace(int x,int y);           //конструктор, размеры матрицы холста
    void addInvert(int x, int y);       //добавить точку в инвертированную матрицу
    void delInvert(int x, int y);       //удалить точку из инвертированной матрицы
    //test
    void addCellCanvas(int x, int y);   //добавить точку в холст
    void delCellCanvas(int x, int y);   //удалить точку из холста
    //
    bool insertFigure(Figure figure, int x, int y);  //вставить фигуру в холст
    void deleteFigure();                             //удалить фигуру из холста
    //
    bool getStateInvertCanvas(int x, int y);//c 0    //получить состо€ние €чейки инвертированного холста
    bool getStateCanvas(int x, int y);//c 0          //получить состо€ние €чейки холста
    //
    int getSizeCanvasX();                            //получить размер холста X
    int getSizeCanvasY();                            //получить размер холста Y
    //
    int getSizeFigures();                            //получить размер массива ¬—“ј¬Ћ≈ЌЌџ’ фигур
    FigurePosition* getFigureP(int num);             //получить фигуру из списка ¬—“ј¬Ћ≈ЌЌџ’ фигур
    //
//    int getSizeExistFigures();                       //получить размер массива —”ў≈—“¬”ёў»’ фигур
//    Figure* getExistFigures(int num);               //получить фигуру из списка —”ў≈—“¬”ёў»’ фигур
private:
	//mb QBitArray ?
    QVector<QVector<bool> > canvas;                 //двумерный булеан
    QVector<QVector<bool> > invert_canvas;          //двумерна€ инверси€ булеана
    QVector<FigurePosition* >  figures;             //массив вставленных фигур
//    QVector<Figure* > exist_figures;               //существующие фигуры
//--------
    int numberShiftCoordinate(int coordinate_one, int coordinate_two);  //возвращает рассто€ние между двум€ координатами
    Point* shiftCoordinatePoint(Point *center_point, int shift_x, int shift_y);   //возвращает точку со смещенными координатами
    bool compareFreePosition(FigurePosition *Fpos);///< @todo дописать
};

#endif // CANVASPLACE_H
