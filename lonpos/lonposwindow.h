#ifndef LONPOSWINDOW_H
#define LONPOSWINDOW_H
//�������� ����
#include <QMainWindow>
#include "canvasplace.h"
#include "lonposcanvaswidget.h"

namespace Ui {
class LonposWindow;
};

class LonposWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit LonposWindow(QWidget *parent = 0);
    ~LonposWindow();    
private:
    Ui::LonposWindow *ui;
    canvasPlace *canvasP;           //��������� �� ������ ������
    QVector<Figure*>  Figvect;      //���������� ������ ������������ ������ (�������� �������������)
};

#endif // LONPOSWINDOW_H
