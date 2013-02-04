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
    explicit FigureWidget(QWidget *parent = 0);

signals:
protected:
    void paintEvent(QPaintEvent *);
    
public slots:
private:
    int x_wight,y_height;
    
};

#endif // FIGUREWIDGET_H
