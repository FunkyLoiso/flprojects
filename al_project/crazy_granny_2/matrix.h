#ifndef MATRIX_H
#define MATRIX_H
#include <stdlib.h>
#include <stdio.h>
#include <QTime>

class matrix
{
public:
    matrix(int number);    //�����������
    void arrayView();   //������� �������
    void arrayAutoFilling();  //�������������� ���������� �������
    void arrayFillingSelf();  //������ ���������� �������
    int sumLine();         //������������ �����
private:
    char ver;
    bool v;

    struct matrixStruc
    {
        char *matrixLine;
        int x,y;
    };

    matrixStruc matrixLocal;

};

#endif // MATRIX_H
