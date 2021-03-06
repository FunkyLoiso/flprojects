#include "lonposwindow.h"
#include "ui_lonposwindow.h"
#include "QLabel"
#include "QVBoxLayout"
#include "QFormLayout"
#include "QGridLayout"
#include "qbitarray.h"
#include "figurewidget.h"

LonposWindow::
LonposWindow(QWidget *parent) :
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
    //�������� �����
    //������ ������ ������������� � � ������ ������������� ����� ������� �������  !!
    //  +
    // +++
    //  +
    Figvect.append(new Figure());
    Figure* tmp = Figvect.last();
    tmp->insertPoint(1,1);
    tmp->insertPoint(1,2);
    tmp->insertPoint(1,0);
    tmp->insertPoint(2,1);
    tmp->insertPoint(0,1);
    // ++
    // +
    // +
    // +
    Figvect.append(new Figure());
    tmp = Figvect.last();
    tmp->clearPoints();
    tmp->insertPoint(0,0);
    tmp->insertPoint(1,0);
    tmp->insertPoint(2,0);
    tmp->insertPoint(0,1);
    tmp->insertPoint(0,2);
    //+
    //+
    //+
    //+
    Figvect.append(new Figure());
    tmp = Figvect.last();
    tmp->clearPoints();
    tmp->insertPoint(0,0);
    tmp->insertPoint(0,1);
    tmp->insertPoint(0,2);
    tmp->insertPoint(0,3);
    //+
    //++
    //+
    //+
    Figvect.append(new Figure());
    tmp = Figvect.last();
    tmp->clearPoints();
    tmp->insertPoint(0,0);
    tmp->insertPoint(0,1);
    tmp->insertPoint(1,1);
    tmp->insertPoint(0,2);
    tmp->insertPoint(0,3);
    //++
    //+
    //++
    Figvect.append(new Figure());
    tmp = Figvect.last();
    tmp->clearPoints();
    tmp->insertPoint(0,0);
    tmp->insertPoint(0,1);
    tmp->insertPoint(0,2);
    tmp->insertPoint(1,0);
    tmp->insertPoint(1,2);
    //+
    //++
    //++
    Figvect.append(new Figure());
    tmp = Figvect.last();
    tmp->clearPoints();
    tmp->insertPoint(0,0);
    tmp->insertPoint(0,1);
    tmp->insertPoint(0,2);
    tmp->insertPoint(1,1);
    tmp->insertPoint(1,2);
    //++
    //+
    Figvect.append(new Figure());
    tmp = Figvect.last();
    tmp->clearPoints();
    tmp->insertPoint(0,0);
    tmp->insertPoint(0,1);
    tmp->insertPoint(1,0);
    // ++
    //++
    //+
    Figvect.append(new Figure());
    tmp = Figvect.last();
    tmp->clearPoints();
    tmp->insertPoint(0,2);
    tmp->insertPoint(0,1);
    tmp->insertPoint(1,1);
    tmp->insertPoint(1,0);
    tmp->insertPoint(2,0);
    //++
    //+
    //+
    Figvect.append(new Figure());
    tmp = Figvect.last();
    tmp->clearPoints();
    tmp->insertPoint(1,0);
    tmp->insertPoint(0,0);
    tmp->insertPoint(0,1);
    tmp->insertPoint(0,2);
    //++
    //+
    //+
    //+
    Figvect.append(new Figure());
    tmp = Figvect.last();
    tmp->clearPoints();
    tmp->insertPoint(1,0);
    tmp->insertPoint(0,0);
    tmp->insertPoint(0,1);
    tmp->insertPoint(0,2);
    tmp->insertPoint(0,3);
    //++
    //++
    Figvect.append(new Figure());
    tmp = Figvect.last();
    tmp->clearPoints();
    tmp->insertPoint(1,0);
    tmp->insertPoint(0,0);
    tmp->insertPoint(0,1);
    tmp->insertPoint(1,1);
    // +
    //++
    //+
    //+
    Figvect.append(new Figure());
    tmp = Figvect.last();
    tmp->clearPoints();
    tmp->insertPoint(1,0);
    tmp->insertPoint(1,1);
    tmp->insertPoint(0,1);
    tmp->insertPoint(0,2);
    tmp->insertPoint(0,3);



//������� �����
//    canvasP->insertFigure(Figvect[1],3,4);


//������������� ��� ������������ ����� gridlayout
    QGridLayout* QGL = new QGridLayout(ui->scrollArea);
    for (int i=0,row=0,column=0;i<Figvect.size();i++,column++)
    {
        if (column>2) {row++;column=0;}
        QGL->addWidget(new FigureWidget(Figvect[i]),row,column);
    }

//������������� ��� ������������ ����� vertlayout
//    QVBoxLayout* QVBL = new QVBoxLayout(ui->scrollArea);
//    for (int i=0;i<Figvect.size();i++)
//    {
//        QVBL->addWidget(new FigureWidget(Figvect[i]));
//    }


//    QVBL->addWidget(new QLabel("LOL2"));
}

LonposWindow::~LonposWindow()
{
    delete ui;
}

