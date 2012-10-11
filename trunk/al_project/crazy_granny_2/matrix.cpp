#include "matrix.h"
matrix::matrix(int x, int y)
{
    QTime midnight(0,0,0);  //создание таймера
    qsrand(midnight.secsTo(QTime::currentTime())); //инициализация генератора случайных чисел

    matrix::matrixLocal.matrixLine = new char[x*y];
    matrixLocal.x=x;
    matrixLocal.y=y;
}

void matrix::arrayView()
{
    for(int chet_y = 0;chet_y<matrixLocal.y;chet_y++)
    {
        for(int chet_x = 0;chet_x<matrixLocal.x;chet_x++)
        {
            std::cout << (int)matrixLocal.matrixLine[((chet_y+1)*matrixLocal.x)-(matrixLocal.x-chet_x-1)-1];   // (текущая строка * общее количество столбцов)-(общее количество столбцов - текущий столбец)
            std::cout << ' ';
        }
        std::cout << '\n';
    }
}

void matrix::arrayAutoFilling()
{
    for(int i = 0;i<matrixLocal.x*matrixLocal.y;i++)
    {
        matrixLocal.matrixLine[i]=qrand()%127;
    }
}

void matrix::arrayFillingSelf()
{
    int temp;
    std::cout << "заполните матрицу" << '\n';
    for(int i = 0;i<matrixLocal.x*matrixLocal.y;i++)
    {
        while(true)
        {
            std::cout << "введите элемент:";
            std::cin >> temp;
            if (std::cin.good())
            {
                if(temp > SCHAR_MAX || temp < 0)
                {
                    std::cout << "введенное число выходит за границы диапазона char." << "\n";
                    std::cin.clear();
                    std::cin.ignore(INT_MAX, '\n');
                    continue;
                }
            }
            if (!std::cin.good() || std::cin.peek() != '\n')
            {
                std::cout << "ошибочная запись, можно вводить только цецые положительные числа в диапазоне 0<=матрица<128" << "\n";
                std::cin.clear();
                std::cin.ignore(INT_MAX, '\n');
                continue;
            }
            matrixLocal.matrixLine[i]=(char)temp;
            break;
        }
    }
}

int  matrix::sumLine(bool ver, int line)
{
    int sum=0;

    if(ver)       //строки
    {
        for(int chet_x = 0;chet_x<matrixLocal.x;chet_x++)
        {
            sum+=(int)matrixLocal.matrixLine[((line+1)*matrixLocal.x)-(matrixLocal.x-chet_x-1)-1];
        }
    }
    if(!ver)      //столбцы
    {
        for(int chet_y = 0;chet_y<matrixLocal.y;chet_y++)
        {
            sum+=(int)matrixLocal.matrixLine[((chet_y+1)*matrixLocal.x)-(matrixLocal.x-line-1)-1];
        }
    }
    return sum;
}


