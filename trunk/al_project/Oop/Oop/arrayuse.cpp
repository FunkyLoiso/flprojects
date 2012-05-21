#include "arrayuse.h"
#include <QtGlobal>
#include "QTime"
#include <iostream>

//using namespace std;

ArrayUse::ArrayUse(): in(stdin),out(stdout)   //конструктор
{

    qsrand((QTime(0,0,0).msecsTo(QTime::currentTime()))); //инифиализация генератора случайных чисел
    ArrayUse::AddMass(2,2);
    ArrayUse::AddMass(1,1);
    ArrayUse::AddMass(3,3);
    ArrayUse::AddMass(4,2);
    ArrayUse::FillInMassRandom(0);
    ArrayUse::FillInMassRandom(1);
    ArrayUse::FillInMassRandom(2);
    ArrayUse::FillInMassRandom(3);
//    std::cout << ArrayUse::TestBorderMatrix(3,true);
//    for (int i=0; i
//         <mass.size();i++)
//    {
//        ArrayUse::FillInMassRandom(i);
//    }
}

bool ArrayUse::ShowMass(int index)    // получить массив по индексу
{
    for (int ordinate = 0;ordinate<mass[index-1].size();++ordinate) //Y
    {
        for (int abscissa = 0;abscissa<mass[index-1][ordinate].size();++abscissa)   //X
        {
            std::cout << mass[index-1][ordinate][abscissa] << " ";
        }
        std::cout << "\n";
    }
    return true;
}

bool ArrayUse::FillInMassRandom(int index) //рандомное заполнение массива по индексу
{

    for (int ordinate = 0;ordinate<mass[index].size();ordinate++)   //X
    {
        for (int abscissa = 0; abscissa<mass[index][ordinate].size();abscissa++) //Y
        {
            mass[index][ordinate][abscissa] = qrand();
        }
    }
    return true;
}

bool ArrayUse::FillInMassSelf(int index, int _abscissa, int _ordinate)
{
    int tmp;
    int lol;
    for (int ordinate = 0;ordinate<mass[index].size();ordinate++)   //X
    {

        lol = mass[index][ordinate].size();
        for (int abscissa = 0; abscissa<mass[index][ordinate].size();abscissa++) //Y
        {
            if (_ordinate==-1 and _abscissa==-1)
            {
                do
                {
                    std::cout << "input[" << ordinate << "][" << abscissa << "]:";
                    std::cin >> tmp;
                }
                while (!std::cin.good());
                mass[index][ordinate][abscissa] = tmp;
            }
            else if (_ordinate==ordinate && _abscissa==abscissa)
                 {
                    do
                    {
                        std::cout << "input[" << ordinate << "]" <<"[" << abscissa << "]:";
                        std::cin >> tmp;
                    }
                    while (!std::cin.good());
                    mass[index][ordinate][abscissa] = tmp;
                 }
        }
    }
    return true;
}

int ArrayUse::TestBorderMatrix(int index, bool test)
{
    if (!test)
    {
       return mass[index-1].size();  //X
    }
    else
    {
       return mass[index-1][0].size(); //Y
    }
    //return true;
}

bool ArrayUse::AddMass(int abscissa, int ordinate)   // добавить новую матрицу в массив матриц
{
    QVector <QVector <int> > value_mass;
    value_mass.resize(ordinate);
    for (int a=0;a<ordinate;a++)
    {
        value_mass[a].resize(abscissa);
    }
    mass.append(value_mass);
    return true;
}

bool ArrayUse::DelMass(int index)   // удаление массива по индексу
{
    mass.remove(index-1);
    return true;
}

bool ArrayUse::CompMass(int index_one, bool ver_one, int line_one, int index_two, bool ver_two, int line_two)    // сравнение матриц
{
    if (ArrayUse::line_sum(index_one,ver_one,line_one)>ArrayUse::line_sum(index_two,ver_two,line_two))
    {
        std::cout << "\n" << ArrayUse::line_sum(index_one,ver_one,line_one) << ">" << ArrayUse::line_sum(index_two,ver_two,line_two) << "\n";
    }
    else if (ArrayUse::line_sum(index_one,ver_one,line_one)==ArrayUse::line_sum(index_two,ver_two,line_two))
         {
            std::cout << "\n" << ArrayUse::line_sum(index_one,ver_one,line_one) << "=" << ArrayUse::line_sum(index_two,ver_two,line_two) << "\n";
         }
    else
    {
        std::cout << "\n" << ArrayUse::line_sum(index_one,ver_one,line_one) << "<" << ArrayUse::line_sum(index_two,ver_two,line_two) << "\n";
    }
    return 0;
}

bool ArrayUse::CheckSizeMatrix(int index, int line_one, bool ver)    //проверка на вывод за пределы матрицы
{
    if (ver)
    {
        if (line_one-1 > mass[index-1][0].size() || line_one-1<0) {return false;}
        return true;
    }
    else
    {
        if (line_one-1 > mass[index-1].size() || line_one-1<0) {return false;}
        return true;
    }
}

int ArrayUse::line_sum(int index, bool ver, int line)
{   int sum =0;
    if (ver)
    {
        for (int abscissa = 0; abscissa<mass[index][line].size();abscissa++) //X
            {
                sum +=mass[index][line][abscissa];
            }
        return sum;
    }
    else
    {
        for (int ordinate = 0;ordinate<mass[index].size();ordinate++)   //Y
        {
            sum +=mass[index][ordinate][line];
        }
        return sum;
    }
}

//int ArrayUse::ordinata(int index, bool ver_two, int line)
//{   int sum =0;
//    for (int ordinate = 0;ordinate<mass[index].size();ordinate++)   //Y
//    {
//        sum +=mass[index][ordinate][line];
//    }
//    return sum;
//}
