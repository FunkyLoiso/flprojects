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
}

void canvasPlace::deleteFigure()
{
}

int canvasPlace::numberShiftCoordinate(int coordinate_one, int coordinate_two)
{
    return abs(coordinate_one)+abs(coordinate_two);
}

int canvasPlace::shiftCoordinate(Point center_point, int x, int y)
{
//    int shift_x = numberShiftCoordinate(center_point/*abs(center_point.getX())+abs(x)*/;
//    int shift_y = /*abs(center_point.getY())+abs(y)*/;
//    if(center_point.getX()>0)
//    {
//        center
//    }
}
