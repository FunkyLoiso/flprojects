#include <iostream>
using namespace std;

#include "DynArray.h"

int main()
{
	setlocale(LC_ALL, "Russian");//��� ������ ��������
	cout << "���������� ����������� ������������� ������� �� �����������. ����� �.�.���-06-08" << endl;

	while(true)
	{
		DynArray arr;
		while(true)
		{
			cout << endl << "������� ����� �����["<< SCHAR_MIN << ", " << SCHAR_MAX << "] ����� ������. Enter ��� ����������, q ��� ������" << endl;
			cout << ">";

			short tmp;
			while(std::cin.good() && std::cin.peek() != '\n')
			{
				std::cin >> tmp;
				if(std::cin.good())
				{
					if(tmp > SCHAR_MAX || tmp < SCHAR_MIN)
					{//���� ����� �� ������� ��������� �������� - ����������
						cout << "������� " << tmp << " ������� �� ������� ����������� ��������� ["<< SCHAR_MIN << ", " << SCHAR_MAX << "] � ����� ��������������!" << endl;
						continue;
					}
					arr.add(tmp);
				}

				if(!std::cin.good())
				{
					std::cin.clear();

					if(cin.peek() == 'q' || cin.peek() == 'Q')
					{//����� �� ���������
						cout << endl << "�� ��������!" << endl;
						return 0;
					}
					//������� ������������ ��������
					cout << "���������� ������������ �������, ��������� ";
					while(std::cin.peek() != ' ' && std::cin.peek() != '\n') cout << (char)std::cin.get();
					cout << " ����� ���������������" << endl;
				}

				if(arr.size() >= SCHAR_MAX)
				{// ���� ��� ������� 127 ���������
					cout << endl << "������� ������������ ���������� ���������, ��������� ����� ���������������. ���������� ����������!" << endl;
					break;
				}
			}

			if(arr.size() == 0)
			{
				cout << "�� ������� �� ������ ��������" << endl;
				break;
			}

			break;
		}
		// ����� ���������������� ������� �� �����
		if(arr.size() > 0)
		{
			arr.sort();// ����������
			cout << "��������������� ������: ";

			char i = 0;
			while(i < arr.size())
			{
				cout << (int)arr[i++] << ' ';
			}
			cout << endl;
		}
		cin.ignore(INT_MAX, '\n');
	}
}