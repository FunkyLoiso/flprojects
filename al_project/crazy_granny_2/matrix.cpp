#include "matrix.h"
#include <iostream>
matrix::matrix(int number)
{
    QTime midnight(0,0,0);  //�������� �������
    qsrand(midnight.secsTo(QTime::currentTime())); //������������� ���������� ��������� �����

    matrix::matrixLocal.matrixLine = new char[number];
}

void matrix::arrayView()
{
    for(int chet_y = 0;chet_y<matrixLocal.y;chet_y++)
    {
        for(int chet_x = 0;chet_x<matrixLocal.x;chet_x++)
        {
            std::cout << (int)matrixLocal.matrixLine[((chet_y+1)*matrixLocal.x)-(matrixLocal.x-chet_x-1)-1];   // (������� ������ * ����� ���������� ��������)-(����� ���������� �������� - ������� �������)
            std::cout << ' ';
        }
        std::cout << '\n';
    }
}

void matrix::arrayAutoFilling()
{
}

void matrix::arrayFillingSelf()
{
}

int  matrix::sumLine()
{
    return 0;
}


