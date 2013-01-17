#include "lonposwindow.h"
#include "ui_lonposwindow.h"

LonposWindow::LonposWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LonposWindow)
{
    ui->setupUi(this);
}

LonposWindow::~LonposWindow()
{
    delete ui;
}
