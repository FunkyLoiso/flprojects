#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
    ui->lineEdit_3->clear();
    one.resize(999);
    two.resize(999);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
//    number tmp1 = number();
    for (int i1 = 0; i1 != 9;i1++)
    {
        for (int i2 = 0; i2 != 9;i2++)
        {
            for (int i3 = 0; i3 != 9;i3++)
            {
                if (i1>0)
                {
                one[i1*100+i2*10+i3-1].setnumber(i1*100+i2*10+i3);
                two[i1*100+i2*10+i3-1].setnumber(i1*100+i2*10+i3);
                ui->textEdit->append(one[i1*100+i2*10+i3-1].getsizenumstr());
                }
            }
        }
    }
}
