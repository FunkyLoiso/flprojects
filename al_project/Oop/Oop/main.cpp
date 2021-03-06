#include <QtCore/QCoreApplication>
#include <iostream>
#include <iomanip>
#include <arrayuse.h>

using namespace std;

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
    ArrayUse matrix;  //�������� �������
    char choice;
    int index,abscissa,ordinata;

    cout << "programm start";
    for (;;)
    {
        choice = menu();
        switch (choice)
        {

        case 's':
                cout << "\n" << "size_matrixs:" << matrix.GetNumberMass() << endl;
                break;

        case 'v':
            if (!matrix.GetNumberMass()) { cout << "\n" <<"matrix not exist \n";   break;}
                do {
                    cout << "enter index:";
                    cin  >> index;
                    if(!cin.good())
                    {
                        cin.clear();
                        cin.ignore(INT_MAX, '\n');
                    }
                }while(index-1>=matrix.GetNumberMass() || index-1<0);
                cout << "\n" << "matrix:\n";
                matrix.ShowMass(index);// << endl;
                break;

        case 'f':
                if (!matrix.GetNumberMass()) {cout << "\n" <<"matrix not exist \n"; break;}

                do {
                    cout << "enter index:";
                    cin  >> index;
                    if(!cin.good())
                    {
                        cin.clear();
                        cin.ignore(INT_MAX, '\n');
                    }
                }while(index-1>=matrix.GetNumberMass() || index-1<0);
                matrix.FillInMassRandom(index-1);// << endl;
                cout << "\n" << "matrix:\n";
                matrix.ShowMass(index);
                break;

        case 'u':
                if (!matrix.GetNumberMass()) {cout << "\n" <<"matrix not exist \n"; break;}

                do {
                    cout << "enter index:";
                    cin  >> index;
                    if(!cin.good())
                    {
                        cin.clear();
                        cin.ignore(INT_MAX, '\n');
                    }
                }while(index-1>=matrix.GetNumberMass() || index-1<0);

                do {
                    cout << "enter abscissa:";
                    cin >>  abscissa;
                    if(!cin.good())
                    {
                        cin.clear();
                        cin.ignore(INT_MAX, '\n');
                    }
                }while(abscissa-1<-1 || abscissa-1>=matrix.TestBorderMatrix(index,true));

                do{
                    cout << "enter ordinata:";
                    cin >> ordinata;
                    if(!cin.good())
                    {
                        cin.clear();
                        cin.ignore(INT_MAX, '\n');
                    }
                }while( ordinata-1 <-1 || ordinata-1>=matrix.TestBorderMatrix(index,false));

                matrix.FillInMassSelf(index-1,abscissa-1,ordinata-1);// << endl;
                cout << "\n" << "matrix:\n";
                matrix.ShowMass(index);
                break;

        case 'a':
                int x,y;
                cout << "line:";
                cin >> x;
                cout << "column:";
                cin >> y;
                matrix.AddMass(y,x);   //��������� ����� �� ������ (������ ��� ���������� �������)
                break;

        case 'd':
                if (!matrix.GetNumberMass()) {cout << "\n" <<"matrix not exist \n"; break;}
                do {
                    cout << "enter index:";
                    cin  >> index;
                    if(!cin.good())
                    {
                        cin.clear();
                        cin.ignore(INT_MAX, '\n');
                    }
                }while(index-1>=matrix.GetNumberMass() || index-1<0);
                matrix.DelMass(index);
                break;

        case 'c':
            if (!matrix.GetNumberMass()) {cout << "\n" <<"matrix not exist \n"; break;}

                int index_one;
                do {
                    cout << "enter mass number one:";
                    cin >> index_one;
                    if(!cin.good())
                    {
                        cin.clear();
                        cin.ignore(INT_MAX, '\n');
                    }
                }while(index_one-1>=matrix.GetNumberMass() || index_one-1<0);

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
                }while(!(matrix.CheckSizeMatrix(index_one, line_one, ch_one)));

                int index_two;
                do {
                    cout << "enter mass number two:";
                    cin >> index_two;
                    if(!cin.good())
                    {
                        cin.clear();
                        cin.ignore(INT_MAX, '\n');
                    }
                }while(index_two-1>=matrix.GetNumberMass() || index_two-1<0);

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
                }while(!(matrix.CheckSizeMatrix(index_two, line_two, ch_two)));

                matrix.CompMass(index_one-1,ch_one,line_one-1,index_two-1,ch_two,line_two-1);
                break;

        case 'q':
            cout << "enter any key";
            return 0;
         }
    }

    return a.exec();
}
