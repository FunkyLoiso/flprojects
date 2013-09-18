#include "lonposwindow.h"
#include "ui_lonposwindow.h"
#include "QLabel"
#include "QVBoxLayout"
#include "figurewidget.h"

LonposWindow::
LonposWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LonposWindow)
{
    ui->setupUi(this);

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
    //фигуры должны располагаться в в правой части графика функций  !!
    //  +
    // +++
    //  +
    Figvect.append(new Figure());
    Figure* tmp = Figvect.last();
//    tmp->clearPoints();
//    tmp->insertPoint(0,0);
//    tmp->insertPoint(0,1);
//    tmp->insertPoint(0,-1);
//    tmp->insertPoint(1,0);
//    tmp->insertPoint(-1,0);
//решение проблемы сдвига фигуры
    tmp->insertPoint(1,1);
    tmp->insertPoint(1,2);
    tmp->insertPoint(1,0);
    tmp->insertPoint(2,1);
    tmp->insertPoint(0,1);
//    tmp->turnFigureMatrix(90);
    // +
    // +
    // +
    // +
    Figvect.append(new Figure());
    tmp = Figvect.last();
    tmp->clearPoints();
    tmp->insertPoint(0,0);
    tmp->insertPoint(0,1);
    tmp->insertPoint(0,2);
    tmp->insertPoint(0,3);
    tmp->insertPoint(-1,-1);
    tmp->turnFigureMatrix(0);
    //

    canvasP->insertFigure(Figvect[1],3,4);


    QVBoxLayout* QVBL = new QVBoxLayout(ui->scrollArea);
    QVBL->addWidget(new FigureWidget(Figvect[0]));
    QVBL->addWidget(new FigureWidget(Figvect[1]));
//    QVBL->addWidget(new QLabel("LOL2"));
    //
}

LonposWindow::~LonposWindow()
{
    delete ui;
}

