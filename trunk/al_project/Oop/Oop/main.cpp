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
        cout << "(A)dd_mass\n";
        cout << "(D)el_mass\n";
        cout << "(C)omparate_mass\n";
        cout << "(Q)uit\n";
        cout << "enter symbol:";
        cin >> ch;
    }while (!strchr("svfadcq",tolower(ch)));
    return ch;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    ArrayUse matrix;  //создание объекта
    char choice;
    int index;

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
                }while(index-1>=matrix.GetNumberMass() || index-1<0);
                cout << "\n" << "matrix:\n";
                matrix.ShowMass(index);// << endl;
                break;
            case 'f':
                if (!matrix.GetNumberMass()) {cout << "\n" <<"matrix not exist \n"; break;}

                do {
                    cout << "enter index:";
                    cin  >> index;
                }while(index-1>=matrix.GetNumberMass() || index-1<0);
                matrix.FillInMassRandom(index-1);// << endl;
                cout << "\n" << "matrix:\n";
                matrix.ShowMass(index);
                break;
            case 'a':
                int x,y;
                cout << "line:";
                cin >> x;
                cout << "column:";
                cin >> y;
                matrix.AddMass(y,x);   //перепутал нужно на оборот (короче тут правильный вариант)
                break;
            case 'd':
                if (!matrix.GetNumberMass()) {cout << "\n" <<"matrix not exist \n"; break;}
                do {
                    cout << "enter index:";
                    cin  >> index;
                }while(index-1>=matrix.GetNumberMass() || index-1<0);
                matrix.DelMass(index);
                break;
            case 'c':
            if (!matrix.GetNumberMass()) {cout << "\n" <<"matrix not exist \n"; break;}

                int index_one;
                do {
                    cout << "enter mass number one:";
                    cin >> index_one;
                }while(index_one-1>=matrix.GetNumberMass() || index_one-1<0);

                int ch_one;
                do {
                    cout << "choice line(1) or column(0):";
                    cin >> ch_one;
                }while(ch_one!=1 && ch_one!=0);

                int line_one;
                do {
                    cout << "enter line:";
                    cin >> line_one;
                }while(!(matrix.CheckSizeMatrix(index_one, line_one, ch_one)));

                int index_two;
                do {
                    cout << "enter mass number two:";
                    cin >> index_two;
                }while(index_two-1>=matrix.GetNumberMass() || index_two-1<0);

                int ch_two;
                do {
                    cout << "choice line(1) or column(0):";
                    cin >> ch_two;
                }while(ch_two!=1 && ch_two!=0);

                int line_two;
                do {
                    cout << "enter line:";
                    cin >> line_two;
                }while(!(matrix.CheckSizeMatrix(index_two, line_two, ch_two)));
                matrix.CompMass(index_one,ch_one,line_one,index_two,ch_two,line_two);
                break;
            case 'q':
            cout << "enter any key";
            return 0;
         }
    }

    return a.exec();
}
