#ifndef MATRIX_H
#define MATRIX_H
#include <matrix.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <QTime>

class matrix
{
public:
    matrix(int x, int y);    //�����������
    void arrayView();   //������� �������
    void arrayAutoFilling();  //�������������� ���������� �������
    void arrayFillingSelf();  //������ ���������� �������
    int sumLine(bool ver, int line); //������������ �����
private:
    struct matrixStruc
    {
        char *matrixLine;
        int x,y;
    };

    matrixStruc matrixLocal;

};

#endif // MATRIX_H
