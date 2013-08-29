#ifndef FIGUREWIDGET_H
#define FIGUREWIDGET_H
//����������� ��������� �����
#include <QWidget>
#include <QPainter>
#include "figureposition.h"

class FigureWidget : public QWidget
{
    Q_OBJECT
public:
    //��� �������� ���������� ������
    explicit FigureWidget(Figure* FigureL, QWidget *parent = 0);
    //������ ��������� ������
    void  setFigure(Figure* FigureL);
    //��� ����� ����������
    void paintForFigure();
signals:
protected:
    //��� ����� ������
    void paintEvent(QPaintEvent *);
    
public slots:
private:
    Figure* paintFigure;
    void pointCircle(int x, int y, int numberX, int numberY, QColor color=QColor(160,152,152));   //���������� ������ � ����� //x � y ��������� � 0
    
};

#endif // FIGUREWIDGET_H
