#ifndef LONPOSWINDOW_H
#define LONPOSWINDOW_H
//корневое окно
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
    canvasPlace *canvasP;           //указатель на объект холста
    QVector<Figure*>  Figvect;      //содержатся вообще существующие фигуры (задается программистом)
};

#endif // LONPOSWINDOW_H
