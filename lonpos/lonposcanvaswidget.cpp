#include "lonposcanvaswidget.h"
#include <qpainter.h>
#include <QtGui>
LonposCanvasWidget::LonposCanvasWidget(QWidget *parent, int x, int y):
    QWidget(parent)
{
    this->x_weight = x;
    this->y_height = y;
}

void LonposCanvasWidget::setSize(int x, int y)
{
    this->x_weight = x;
    this->y_height = y;
}

void LonposCanvasWidget::paintFonCircle(int x, int y)
{
    QPainter paint(this);       //������ ������������
    paint.setRenderHint(QPainter::Antialiasing);
    //����� ���
    paint.save();
    paint.setBrush(Qt::gray);
    paint.setPen(Qt::NoPen);
    paint.drawRect(0,0,this->width(),this->height());   //�������

    //������� ��� �����
    paint.restore();
    paint.setPen(Qt::NoPen);
    paint.setBrush(Qt::lightGray);
    int tmp_x = (this->width()/(x+1));
    int tmp_y = (this->height()/(y+1));
    for(int yi = 1;yi<=y;yi++)
    {
        for(int xi = 1;xi<=x;xi++)
        {
            paint.drawEllipse(QRectF( tmp_x*xi-tmp_x/2,(tmp_y*yi)-tmp_y/2,tmp_x,tmp_y));
        }
    }
}

void LonposCanvasWidget::paintFonCircle(M2B canvas, M2B invert_canvas)
{
    QPainter paint(this);
    paint.setRenderHints(QPainter::Antialiasing);
    paint.save();
    //����� ���
    paint.setBrush(Qt::gray);
    paint.setPen(Qt::NoPen);
    paint.drawRect(0,0,this->width(),this->height());  // �������

    //������� �����

}

void LonposCanvasWidget::paintEvent(QPaintEvent *)
{
    paintFonCircle(x_weight,y_height);
}
