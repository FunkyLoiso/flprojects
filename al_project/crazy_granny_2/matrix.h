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
    matrix(int x, int y);    //конструктор
    void arrayView();   //вывести матрицу
    void arrayAutoFilling();  //автоматическое заполнение матрицы
    void arrayFillingSelf();  //ручное заполнение матрицы
    int sumLine(bool ver, int line); //суммирование линий
private:
    struct matrixStruc
    {
        char *matrixLine;
        int x,y;
    };

    matrixStruc matrixLocal;

};

#endif // MATRIX_H
