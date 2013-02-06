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
    if((x<invert_canvas.size())&&(y<invert_canvas[0].size()))
    {
        invert_canvas[x][y]=true;
    }

}

void canvasPlace::delInvert(int x, int y)
{
    invert_canvas[x][y]=false;
}

void canvasPlace::addCellCanvas(int x, int y)
{
     canvas[x][y]=true;
}

void canvasPlace::delCellCanvas(int x, int y)
{
     canvas[x][y]=false;
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

bool canvasPlace::getStateInvertCanvas(int x, int y)
{
    return invert_canvas[x][y];
}

bool canvasPlace::getStateCanvas(int x, int y)
{
    return canvas[x][y];
}

int canvasPlace::getSizeCanvasX()
{
    return canvas.size();
}

int canvasPlace::getSizeCanvasY()
{
	return canvas[0].size();
}

int canvasPlace::numberShiftCoordinate(int coordinate_one, int coordinate_two)
{
	return abs(coordinate_one - coordinate_two);
}

Point* canvasPlace::shiftCoordinatePoint(/*const*/ Point* point, int shift_x, int shift_y)
{
    Point* P = new Point(point->getX()+shift_x, point->getY()+shift_y);
    return P;
}

bool canvasPlace::compareFreePosition(FigurePosition* Fpos)
{
    for(int i =1;i<=Fpos->getSizePointOfFigure();i++ )
    {
		Point *pt = Fpos->getPointPosition(i);
        if(canvas[pt->getX()][pt->getY()])			{return false; }
        if(invert_canvas[pt->getX()][pt->getY()])	{return false; }
        canvas[pt->getX()][pt->getY()] = true;
    }
    return true;
}

FigurePosition* canvasPlace::getFigureP(int num)
{
	if(num<this->figures.size()) {return this->figures[abs(num)];};
	return 0;
}
