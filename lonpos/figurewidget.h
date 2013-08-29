#ifndef FIGUREWIDGET_H
#define FIGUREWIDGET_H
//отображение имеющихся фигур
#include <QWidget>
#include <QPainter>
#include "figureposition.h"

class FigureWidget : public QWidget
{
    Q_OBJECT
public:
    //при создании передается фигура
    explicit FigureWidget(Figure* FigureL, QWidget *parent = 0);
    //запись указателя фигуры
    void  setFigure(Figure* FigureL);
    //что нужно нарисовать
    void paintForFigure();
signals:
protected:
    //эта хрень рисует
    void paintEvent(QPaintEvent *);
    
public slots:
private:
    Figure* paintFigure;
    void pointCircle(int x, int y, int numberX, int numberY, QColor color=QColor(160,152,152));   //нарисовать кружок в точке //x и y считаются с 0
    
};

#endif // FIGUREWIDGET_H
