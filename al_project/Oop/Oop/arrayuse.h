#ifndef ARRAYUSE_H
#define ARRAYUSE_H
#include <QVector>
#include <QTextIStream>
class ArrayUse
{
public:
    int x,y;
    ArrayUse();    //конструктор
    int  GetNumberMass()  { return mass.size(); };  //получить размер массива матриц
    bool ShowMass(int index);  // получить массив по индексу
    bool FillInMassRandom(int index); //рандомное заполнение массива по индексу
    bool FillInMassSelf(int index, int _ordinate, int _abscissa);  //заполнить матрицу самостоятельно
    int  TestBorderMatrix(int index, bool test);
    bool AddMass(int abscissa, int ordinate);  // добавить новую матрицу в массив матриц
    bool DelMass(int index);  // удаление массива по индексу
    bool CompMass(int index_one, bool ver_one, int line_one, int index_two, bool ver_two, int line_two);  // сравнение матриц
    bool CheckSizeMatrix(int index, int line_one, bool ver);
private:
    int line_sum(int index, bool ver, int line);
    typedef QVector<QVector<int> > Matrixs;
    QVector<Matrixs> mass;
    QTextStream in;
    QTextStream out;
};

#endif // ARRAYUSE_H
