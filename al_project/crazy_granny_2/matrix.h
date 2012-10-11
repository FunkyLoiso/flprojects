#ifndef MATRIX_H
#define MATRIX_H
#include <stdlib.h>
#include <stdio.h>
#include <QTime>

class matrix
{
public:
    matrix(int number);    //конструктор
    void arrayView();   //вывести матрицу
    void arrayAutoFilling();  //автоматическое заполнение матрицы
    void arrayFillingSelf();  //ручное заполнение матрицы
    int sumLine();         //суммирование линий
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
