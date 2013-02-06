#include "lonposwindow.h"
#include "ui_lonposwindow.h"

LonposWindow::LonposWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LonposWindow)
{
    ui->setupUi(this);
    this->canvasP = new canvasPlace(9,9);
	ui->lonposCanvasWidget->setcanvasPlace(this->canvasP);
    //���������� ������� ����������
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
    canvasP->addCellCanvas(3,4);
    //�������� �����
//    Figure *tmp1 = new Figure();
//    tmp1.
//    Figvect.append();
}

LonposWindow::~LonposWindow()
{
    delete ui;
}

