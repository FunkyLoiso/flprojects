#ifndef LONPOSCANVASWIDGET_H
#define LONPOSCANVASWIDGET_H
//����������� ��������� ������
#include <QtWidgets/qwidget.h>
#include <QPainter>
#include "canvasplace.h"

class LonposCanvasWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LonposCanvasWidget( QWidget *parent = 0/*, int x = 9, int y = 9*/);
    void setSize(int x, int y);
//    void paintFonCircle(Qt::GlobalColor fon, Qt::GlobalColor circle, int x, int y);
    bool paintFonCircle();                        //���������  ���� ��� ���������� � ����� � ���������� ������
    void setcanvasPlace(canvasPlace* canvasLCW);  //��������� ��������� �� �����
signals:
    
public slots:

protected:
    void paintEvent(QPaintEvent *);              //����������� ����� ��� ����������� (��� ����� ������)

private:
    QColor colorC;
    canvasPlace* canvasLCW;                      //��������� �� ��������� �����
    //���������� ������ � ����� x � y, ���������� � 0
    void pointCircle(int x, int y, int numberX, int numberY, QColor color=QColor(160,152,152));
};

#endif // LONPOSCANVASWIDGET_H
