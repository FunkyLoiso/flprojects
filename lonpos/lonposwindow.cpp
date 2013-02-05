#include "lonposwindow.h"
#include "ui_lonposwindow.h"

LonposWindow::LonposWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LonposWindow)
{
    ui->setupUi(this);
    this->canvas = new canvasPlace(9,9);
    //заполнение матрицы блокировки
    canvas->addInvert(1,7);
    //    canvas->addInvert(1,8);
    //    canvas->addInvert(1,9);
    //    canvas->addInvert(2,8);
    //    canvas->addInvert(2,9);
    //    canvas->addInvert(3,9);
    //    canvas->addInvert(7,1);
    //    canvas->addInvert(8,1);
    //    canvas->addInvert(8,2);
    //    canvas->addInvert(9,1);
    //    canvas->addInvert(9,2);
    //    canvas->addInvert(9,3);
    //    canvas->addInvert(6,8);
    //    canvas->addInvert(6,9);
    //    canvas->addInvert(7,7);
    //    canvas->addInvert(7,8);
    //    canvas->addInvert(7,9);
    //    canvas->addInvert(8,6);
    //    canvas->addInvert(8,7);
    //    canvas->addInvert(8,8);
    //    canvas->addInvert(8,9);
    //    canvas->addInvert(9,6);
    //    canvas->addInvert(9,7);
    //    canvas->addInvert(9,8);
    //    canvas->addInvert(9,9);
    //описание фигур
//    Figure *tmp1 = new Figure();
//    tmp1.
//    Figvect.append();
}

LonposWindow::~LonposWindow()
{
    delete ui;
}

