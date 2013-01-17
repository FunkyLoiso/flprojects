#include "canvasplace.h"

canvasPlace::canvasPlace(int x, int y)
{
    canvas.resize(x);
    invert_canvas.resize(x);
    for (int i=canvas.size()-1;i!=0;i--)
    {
        canvas[i].resize(y);
        invert_canvas[i].resize(y);
        for (int u=canvas[i].size()-1;u!=0;i--)
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
    for(int i = 0;i<figure.getSizePointOfFigure();i++)
    {
        Fpos->insertPointPosition(shiftCoordinatePoint(figure.getPoint(i+1),shift_coordinate_x,shift_coordinate_y));
    }
    canvasPlace::figures.append(Fpos);
}

void canvasPlace::deleteFigure()
{
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
