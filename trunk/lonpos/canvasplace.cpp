#include "canvasplace.h"

canvasPlace::canvasPlace(int x, int y)
{
    canvas.resize(x);
    invert_canvas.resize(x);
    for (int i=0;i<canvas.size();i++)
    {
        canvas[i].resize(y);
        invert_canvas[i].resize(y);
        for (int u=0;u<canvas[i].size();u++)
        {
            canvas[i][u]=false;
            invert_canvas[i][u]=false;
        }
    }
}

void canvasPlace::addInvert(int x, int y)
{
    invert_canvas[x][y]=true;
}

void canvasPlace::delInvert(int x, int y)
{
    invert_canvas[x][y]=false;
}

bool canvasPlace::insertFigure(Figure figure, int x, int y)
{
    int shift_coordinate_x = numberShiftCoordinate(figure.getPoint(1)->getX(),x);
    int shift_coordinate_y = numberShiftCoordinate(figure.getPoint(1)->getY(),y);
    FigurePosition* Fpos = new FigurePosition(figure);
    for(int i = 1;i<=figure.getSizePointOfFigure();i++)
    {
        Fpos->insertPointPosition(shiftCoordinatePoint(figure.getPoint(i),shift_coordinate_x,shift_coordinate_y));
    }
    if(!compareFreePosition(Fpos) ) {return false;}
    canvasPlace::figures.append(Fpos);
    return true;
}

void canvasPlace::deleteFigure()
{
}

QVector<QVector<bool> > canvasPlace::getCanvas()
{
    return this->canvas;
}

QVector<QVector<bool> > canvasPlace::getInvertCanvas()
{
    return this->invert_canvas;
}

int canvasPlace::numberShiftCoordinate(int coordinate_one, int coordinate_two)
{
    return abs(coordinate_one)+abs(coordinate_two);
}

Point* canvasPlace::shiftCoordinatePoint(/*const*/ Point* point, int shift_x, int shift_y)
{
    Point* P = new Point(point->getX()+shift_x,point->getY()+shift_y);
    return P;
}

bool canvasPlace::compareFreePosition(FigurePosition* Fpos)
{
    for(int i =1;i<=Fpos->getSizePointOfFigure();i++ )
    {
        if(canvas[Fpos->getPointPosition(i)->getX()][Fpos->getPointPosition(i)->getY()]) {return false; }
        if(invert_canvas[Fpos->getPointPosition(i)->getX()][Fpos->getPointPosition(i)->getY()]) {return false; }
        canvas[Fpos->getPointPosition(i)->getX()][Fpos->getPointPosition(i)->getY()] = true;
    }
    return true;
}
