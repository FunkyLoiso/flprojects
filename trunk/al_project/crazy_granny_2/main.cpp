#include <QtCore/QCoreApplication>
#include <matrix.h>
using namespace std;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    int l1,l2,x1,x2,y1,y2;
    char ver1,ver2;
    bool v1,v2;


    setlocale(LC_ALL, "Russian");
//------------------------------------------------------
//�������� ������� ������� (main)
    while(true)
    {
        cout << "������� ���������� ����� ������� �1:";
        cin >> x1;
        if (cin.good())
        {
            if(x1 > SCHAR_MAX || x1 < 0)
            {
                cout << "��������� ����� ������� �� ������� ��������� char." << "\n";
                cin.clear();
                cin.ignore(INT_MAX, '\n');
                continue;
            }
        }
        if (!cin.good() || cin.peek() != '\n')
        {
            cout << "��������� ������, ����� ������� ������ ����� ������������� ����� � ��������� 0<=�������<128" << "\n";
            cin.clear();
            cin.ignore(INT_MAX, '\n');
            continue;
        }
        break;
    }
    while(true)
    {
        cout << "������� ���������� �������� ������� �1:";
        cin >> y1;
        if (cin.good())
        {
            if(y1 > SCHAR_MAX || y1 < 0)
            {
                cout << "��������� ����� ������� �� ������� ��������� char." << "\n";
                cin.clear();
                cin.ignore(INT_MAX, '\n');
                continue;
            }
        }
        if (!cin.good() || cin.peek() != '\n')
        {
            cout << "��������� ������, ����� ������� ������ ����� ������������� ����� � ��������� 0<=�������<128" << "\n";
            cin.clear();
            cin.ignore(INT_MAX, '\n');
            continue;
        }
        break;
    }

    matrix matrix_1(x1,y1);
//------------------------------------------------------
//�������� ������� ������� (main)
    while(true)
    {
        cout << "������� ���������� ����� ������� �2:";
        cin >> x2;
        if (cin.good())
        {
            if(x2 > SCHAR_MAX || x2 < 0)
            {
                cout << "��������� ����� ������� �� ������� ��������� char." << "\n";
                cin.clear();
                cin.ignore(INT_MAX, '\n');
                continue;
            }
        }
        if (!cin.good() || cin.peek() != '\n')
        {
            cout << "��������� ������, ����� ������� ������ ����� ������������� ����� � ��������� 0<=�������<128" << "\n";
            cin.clear();
            cin.ignore(INT_MAX, '\n');
            continue;
        }
        break;
    }
    while(true)
    {
        cout << "������� ���������� �������� ������� �2:";
        cin >> y2;
        if (cin.good())
        {
            if(y2 > SCHAR_MAX || y2 < 0)
            {
                cout << "��������� ����� ������� �� ������� ��������� char." << "\n";
                cin.clear();
                cin.ignore(INT_MAX, '\n');
                continue;
            }
        }
        if (!cin.good() || cin.peek() != '\n')
        {
            cout << "��������� ������, ����� ������� ������ ����� ������������� ����� � ��������� 0<=�������<128" << "\n";
            cin.clear();
            cin.ignore(INT_MAX, '\n');
            continue;
        }
        break;
    }

    matrix matrix_2(x2,y2);
//------------------------------------------------------
//����� ���������
    while(true)
    {
        cout << "������� ����� ����� ��� ���������, ������� �1:";
        cin >> l1;
        if (cin.good())
        {
            if(y2 > SCHAR_MAX || y2 < 0)
            {
                cout << "��������� ����� ������� �� ������� ��������� char." << "\n";
                cin.clear();
                cin.ignore(INT_MAX, '\n');
                continue;
            }
        }
        if (!cin.good() || cin.peek() != '\n')
        {
            cout << "��������� ������, ����� ������� ������ ����� ������������� ����� � ��������� 0<=�������<128" << "\n";
            cin.clear();
            cin.ignore(INT_MAX, '\n');
            continue;
        }
        break;
    }
    while(true)
    {
        cout << "�������� �������(n) ��� ������(y),������� �1:";
        cin >> ver1;
        if (ver1 == 'y' || ver1 == 'n')
        {
            if(ver1 == 'y')
            {
                v1=true;
            }
            if(ver1 == 'n')
            {
                v1=false;
            }
            break;
        }
        cout << "������� ������ �����" << "\n";
        cin.clear();
        cin.ignore(INT_MAX, '\n');
        continue;
    }
    while(true)
    {
        cout << "������� ����� ����� ��� ���������, ������� �2:";
        cin >> l2;
        if (cin.good())
        {
            if(y2 > SCHAR_MAX || y2 < 0)
            {
                cout << "��������� ����� ������� �� ������� ��������� char." << "\n";
                cin.clear();
                cin.ignore(INT_MAX, '\n');
                continue;
            }
        }
        if (!cin.good() || cin.peek() != '\n')
        {
            cout << "��������� ������, ����� ������� ������ ����� ������������� ����� � ��������� 0<=�������<128" << "\n";
            cin.clear();
            cin.ignore(INT_MAX, '\n');
            continue;
        }
        break;
    }
    while(true)
    {
        cout << "�������� �������(n) ��� ������(y),������� �2:";
        cin >> ver2;
        if (ver2 != 'y' || ver2 != 'n')
        {
            if(ver2 == 'y')
            {
                v2=true;
            }
            if(ver2 == 'n')
            {
                v2=false;
            }
            break;
        }
        cout << "������� ������ �����" << "\n";
        cin.clear();
        cin.ignore(INT_MAX, '\n');
        continue;
    }
//------------------------------------------------------
//�������������� ���������� ������� (main)
    matrix_1.arrayAutoFilling();
    matrix_2.arrayAutoFilling();
//------------------------------------------------------
//������ ���������� ������� (main)
//    matrix_1.arrayFillingSelf();
//    matrix_2.arrayFillingSelf();
//------------------------------------------------------
//����� �������� (main)
    cout << '\n';
    matrix_1.arrayView();
    cout << '\n';
    matrix_2.arrayView();
//------------------------------------------------------
//���������
    if (matrix_1.sumLine(v1,l1) > matrix_2.sumLine(v2,l2))  //  >
    {
        cout << "\n" << matrix_1.sumLine(v1,l1) << ">" << matrix_2.sumLine(v2,l2) << "\n";
    }
    if (matrix_1.sumLine(v1,l1) == matrix_2.sumLine(v2,l2)) //  =
    {
        cout << "\n" << matrix_1.sumLine(v1,l1) << "=" << matrix_2.sumLine(v2,l2) << "\n";
    }
    if (matrix_1.sumLine(v1,l1) < matrix_2.sumLine(v2,l2))  //  <
    {
        cout << "\n" << matrix_1.sumLine(v1,l1) << "<" << matrix_2.sumLine(v2,l2) << "\n";
    }
//------------------------------------------------------
    return a.exec();
}
