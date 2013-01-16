#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
//    number tmp1 = number();
    for (int i1 = 0; i1 <= 9;i1++)
    {
        for (int i2 = 0; i2 <= 9;i2++)
        {
            for (int i3 = 0; i3 <= 9;i3++)
            {
                if (i1>0)
                {
//                one[i1*100+i2*10+i3-1].setnumber(i1*100+i2*10+i3);
//                two[i1*100+i2*10+i3-1].setnumber(i1*100+i2*10+i3);
//                ui->textEdit->append(one[i1*100+i2*10+i3-1].getnumstr()+"="+QString("%1").arg(one[i1*100+i2*10+i3-1].sumnumber()));
                    number num(i1*100+i2*10+i3);
                    one.append(num);
//                    ui->textEdit->append(one.last().getnumstr()+"="+QString("%1").arg(one.last().sumnumber()));
                }
            }
        }
    }
    for (int i = 0;i<one.size();i++)
    {
        for(int ii = 0;ii<one.size();ii++)
        {
            if ( (one[i].sumnumber() == one[ii].sumnumber()) && (one[i].getfullnumber() != one[ii].getfullnumber()) )
            {
                if (one[i].getfullnumber()>one[ii].getfullnumber())
                {
                    int tmp = one[i].getfullnumber()-one[ii].getfullnumber();
                    if (tmp == 899 or tmp == 810 or tmp == 801 or tmp == 792 or tmp == 783)
                    {
                        ui->textEdit->append(one[i].getnumstr()+"-"+one[ii].getnumstr()+"="+QString("%1").arg(tmp));
                    }
                }
                else
                {
                    int tmp = one[ii].getfullnumber()-one[i].getfullnumber();
                    if (tmp == 899 or tmp == 810 or tmp == 801 or tmp == 792 or tmp == 783)
                    {
                        ui->textEdit->append(one[ii].getnumstr()+"-"+one[i].getnumstr()+"="+QString("%1").arg(tmp));
                    }
                }
            }
        }
    }
}


