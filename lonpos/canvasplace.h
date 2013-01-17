#ifndef CANVASPLACE_H
#define CANVASPLACE_H

#include "figureposition.h"

class canvasPlace
{
public:
    canvasPlace(int x,int y);
    void addInvert(int x, int y);
    void delInvert(int x, int y);
    bool insertFigure(Figure figure, int x, int y);
    void deleteFigure();
private:
    QVector<QVector<bool> > canvas;
    QVector<QVector<bool> > invert_canvas;
    QVector<FigurePosition* >  figures;
//--------
    int numberShiftCoordinate(int coordinate_one, int coordinate_two);
    int shiftCoordinate(Point center_point, int x, int y);
};

#endif // CANVASPLACE_H
