#include "lonposwindow.h"
#include "ui_lonposwindow.h"
#include "QLabel"
#include "QVBoxLayout"

LonposWindow::
LonposWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LonposWindow)
{
    ui->setupUi(this);
//    QVBoxLayout* QVBL = new QVBoxLayout(ui->scrollArea);
//    QVBL->addWidget(new QLabel("LOL1"));
//    QVBL->addWidget(new QLabel("LOL2"));
    //
    this->canvasP = new canvasPlace(9,9);
	ui->lonposCanvasWidget->setcanvasPlace(this->canvasP);
    //заполнение матрицы блокировки
    canvasP->addInvert(0,6);
    canvasP->addInvert(0,7);
    canvasP->addInvert(0,8);
    canvasP->addInvert(1,7);
    canvasP->addInvert(1,8);
    canvasP->addInvert(2,8);
    canvasP->addInvert(6,0);
    canvasP->addInvert(7,0);
    canvasP->addInvert(7,1);
    canvasP->addInvert(8,0);
    canvasP->addInvert(8,1);
    canvasP->addInvert(8,2);
    canvasP->addInvert(5,7);
    canvasP->addInvert(5,8);
    canvasP->addInvert(6,6);
    canvasP->addInvert(6,7);
    canvasP->addInvert(6,8);
    canvasP->addInvert(7,5);
    canvasP->addInvert(7,6);
    canvasP->addInvert(7,7);
    canvasP->addInvert(7,8);
    canvasP->addInvert(8,5);
    canvasP->addInvert(8,6);
    canvasP->addInvert(8,7);
    canvasP->addInvert(8,8);
    canvasP->addInvert(2,3);
//    canvasP->addCellCanvas(3,4);
//    canvasP->addCellCanvas(3,3);
    //описание фигур
    Figure* tmp = new Figure();
    //  +
    // +++
    //  +
    tmp->insertPoint(0,0);
    tmp->insertPoint(0,1);
    tmp->insertPoint(0,-1);
    tmp->insertPoint(-1,0);
    tmp->insertPoint(1,0);
    Figvect.append(tmp);
}

LonposWindow::~LonposWindow()
{
    delete ui;
}

