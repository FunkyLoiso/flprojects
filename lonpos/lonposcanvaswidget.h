#ifndef LONPOSCANVASWIDGET_H
#define LONPOSCANVASWIDGET_H
//����������� ��������� ������
#include <QWidget>
#include <QPainter>

class LonposCanvasWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LonposCanvasWidget( QWidget *parent = 0, int x = 9, int y = 9);
    bool setSize(int x, int y);
signals:
    
public slots:

protected:
    void paintEvent(QPaintEvent *);              //����������� ����� ��� �����������

private:
    int x_weight,y_height;

    
};

#endif // LONPOSCANVASWIDGET_H
