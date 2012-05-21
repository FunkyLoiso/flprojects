#include <QtCore/QCoreApplication>
#include <iostream>
#include <QVector>

using namespace std;

int x,y;
typedef QVector<QVector<int> > Matrixs;
typedef QVector<Matrixs>  my_matrix;


//int  GetNumberMass()  { return  };  //получить размер массива матриц
bool ShowMass(my_matrix &mass,int index)  // получить массив по индексу
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
bool FillInMassRandom(my_matrix &mass,int index) //рандомное заполнение массива по индексу
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
bool FillInMassSelf(my_matrix &mass,int index, int _ordinate, int _abscissa)  //заполнить матрицу самостоятельно
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

int  TestBorderMatrix(my_matrix &mass,int index, bool test)
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

bool AddMass(my_matrix &mass,int abscissa, int ordinate)  // добавить новую матрицу в массив матриц
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

bool DelMass(my_matrix &mass,int index)  // удаление массива по индексу
{
    mass.remove(index-1);
    return true;
}

int line_sum(my_matrix &mass,int index, bool ver, int line)
{
    int sum =0;
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

bool CompMass(my_matrix &mass,int index_one, bool ver_one, int line_one, int index_two, bool ver_two, int line_two)  // сравнение матриц
{
    if (line_sum(mass,index_one,ver_one,line_one)>line_sum(mass,index_two,ver_two,line_two))
    {
        std::cout << "\n" << line_sum(mass,index_one,ver_one,line_one) << ">" << line_sum(mass,index_two,ver_two,line_two) << "\n";
    }
    else if (line_sum(mass,index_one,ver_one,line_one)==line_sum(mass,index_two,ver_two,line_two))
         {
            std::cout << "\n" << line_sum(mass,index_one,ver_one,line_one) << "=" << line_sum(mass,index_two,ver_two,line_two) << "\n";
         }
    else
    {
        std::cout << "\n" << line_sum(mass,index_one,ver_one,line_one) << "<" << line_sum(mass,index_two,ver_two,line_two) << "\n";
    }
    return 0;
}

bool CheckSizeMatrix(my_matrix &mass,int index, int line_one, bool ver)
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


char menu()
{
    char ch;
    cout << "\n";
    do  {
//        system("cls");
        cout << "(S)ize_the_matrix_mass\n";
        cout << "(V)iew_the_matrix_index\n";
        cout << "(F)ill_matrix_random_number\n";
        cout << "(U)serFillInMass";
        cout << "(A)dd_mass\n";
        cout << "(D)el_mass\n";
        cout << "(C)omparate_mass\n";
        cout << "(Q)uit\n";
        cout << "enter symbol:";
        cin >> ch;
    }while (!strchr("svfuadcq",tolower(ch)));
    return ch;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    char choice;
    int index,abscissa,ordinata;
    my_matrix mass;

    cout << "programm start";
    for (;;)
    {
        choice = menu();
        switch (choice)
        {
            case 's':
            cout << "\n" << "size_matrixs:" << mass.size() << endl;
                break;

        case 'v':
            if (!mass.size()) { cout << "\n" <<"matrix not exist \n";   break;}
                do {
                    cout << "enter index:";
                    cin  >> index;
                    if(!cin.good())
                    {
                        cin.clear();
                        cin.ignore(INT_MAX, '\n');
                    }
                }while(index-1>=mass.size() || index-1<0);
                cout << "\n" << "matrix:\n";
                ShowMass(mass,index);// << endl;
                break;

        case 'f':
                if (!mass.size()) {cout << "\n" <<"matrix not exist \n"; break;}

                do {
                    cout << "enter index:";
                    cin  >> index;
                    if(!cin.good())
                    {
                        cin.clear();
                        cin.ignore(INT_MAX, '\n');
                    }
                }while(index-1>=mass.size() || index-1<0);
                FillInMassRandom(mass,index-1);// << endl;
                cout << "\n" << "matrix:\n";
                ShowMass(mass,index);
                break;

        case 'u':
                if (!mass.size()) {cout << "\n" <<"matrix not exist \n"; break;}

                do {
                    cout << "enter index:";
                    cin  >> index;
                    if(!cin.good())
                    {
                        cin.clear();
                        cin.ignore(INT_MAX, '\n');
                    }
                }while(index-1>=mass.size() || index-1<0);

                do {
                    cout << "enter abscissa:";
                    cin >>  abscissa;
                    if(!cin.good())
                    {
                        cin.clear();
                        cin.ignore(INT_MAX, '\n');
                    }
                }while(abscissa-1<-1 || abscissa-1>=TestBorderMatrix(mass,index,true));

                do{
                    cout << "enter ordinata:";
                    cin >> ordinata;
                    if(!cin.good())
                    {
                        cin.clear();
                        cin.ignore(INT_MAX, '\n');
                    }
                }while( ordinata-1 <-1 || ordinata-1>=TestBorderMatrix(mass,index,false));

                FillInMassSelf(mass,index-1,abscissa-1,ordinata-1);// << endl;
                cout << "\n" << "matrix:\n";
                ShowMass(mass,index);
                break;

        case 'a':
                int x,y;
                cout << "line:";
                cin >> x;
                cout << "column:";
                cin >> y;
                AddMass(mass,y,x);   //перепутал нужно на оборот (короче тут правильный вариант)
                break;

        case 'd':
                if (!mass.size()) {cout << "\n" <<"matrix not exist \n"; break;}
                do {
                    cout << "enter index:";
                    cin  >> index;
                    if(!cin.good())
                    {
                        cin.clear();
                        cin.ignore(INT_MAX, '\n');
                    }
                }while(index-1>=mass.size() || index-1<0);
                DelMass(mass,index);
                break;

        case 'c':
            if (!mass.size()) {cout << "\n" <<"matrix not exist \n"; break;}

                int index_one;
                do {
                    cout << "enter mass number one:";
                    cin >> index_one;
                    if(!cin.good())
                    {
                        cin.clear();
                        cin.ignore(INT_MAX, '\n');
                    }
                }while(index_one-1>=mass.size() || index_one-1<0);

                int ch_one;
                do {
                    cout << "choice line(1) or column(0):";
                    cin >> ch_one;
                    if(!cin.good())
                    {
                        cin.clear();
                        cin.ignore(INT_MAX, '\n');
                    }
                }while(ch_one!=1 && ch_one!=0);

                int line_one;
                do {
                    cout << "enter line:";
                    cin >> line_one;
                    if(!cin.good())
                    {
                        cin.clear();
                        cin.ignore(INT_MAX, '\n');
                    }
                }while(!(CheckSizeMatrix(mass,index_one, line_one, ch_one)));

                int index_two;
                do {
                    cout << "enter mass number two:";
                    cin >> index_two;
                    if(!cin.good())
                    {
                        cin.clear();
                        cin.ignore(INT_MAX, '\n');
                    }
                }while(index_two-1>=mass.size() || index_two-1<0);

                int ch_two;
                do {
                    cout << "choice line(1) or column(0):";
                    cin >> ch_two;
                    if(!cin.good())
                    {
                        cin.clear();
                        cin.ignore(INT_MAX, '\n');
                    }
                }while(ch_two!=1 && ch_two!=0);

                int line_two;
                do {
                    cout << "enter line:";
                    cin >> line_two;
                    if(!cin.good())
                    {
                        cin.clear();
                        cin.ignore(INT_MAX, '\n');
                    }
                }while(!(CheckSizeMatrix(mass,index_two, line_two, ch_two)));

                CompMass(mass,index_one-1,ch_one,line_one-1,index_two-1,ch_two,line_two-1);
                break;

        case 'q':
            cout << "enter any key";
            return 0;
         }
    }


    
    return a.exec();
}
