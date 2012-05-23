#include <signal.h>
#include <iostream>
using namespace std;

#include "CocktailSort.h"

static const char INIT_SIZE = 10;
static const char k = 2;

// ��������� ���������� �������� � ������
void insert(char val, char*& arr, char& size, unsigned char& allocSize)
{
	if(size >= allocSize)
	{
		allocSize += allocSize/k;
		arr = (char*)realloc(arr, allocSize * sizeof(char));
	}
	arr[size++] = val;
}

int main()
{
	unsigned char allocSize = INIT_SIZE;	//���������� ������������
	char size = 0;							//����� ��������� � �������
	char* arr = (char*)malloc(allocSize * sizeof(char));

	setlocale(LC_ALL, "Russian");//��� ������ ��������
	cout << "���������� ����������� ������������� ������� �� �����������. ����� �.�.���-06-08" << endl;
	
	while(true)
	{
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
					insert(tmp, arr, size, allocSize);
				}

				if(!std::cin.good())
				{
					std::cin.clear();

					if(cin.peek() == 'q' || cin.peek() == 'Q')
					{//����� �� ���������
						free(arr);
						cout << endl << "�� ��������!" << endl;
						return 0;
					}
					//������� ������������ ��������
					cout << "���������� ������������ �������, ��������� ";
					while(std::cin.peek() != ' ' && std::cin.peek() != '\n') cout << (char)std::cin.get();
					cout << " ����� ���������������" << endl;
				}
				
				if(size >= SCHAR_MAX)
				{// ���� ��� ������� 127 ���������
					cout << endl << "������� ������������ ���������� ���������, ��������� ����� ���������������. ���������� ����������!" << endl;
					break;
				}
			}

			if(size == 0)
			{
				cout << "�� ������� �� ������ ��������" << endl;
				break;
			}

			break;
		}
		// ����� ���������������� ������� �� �����
		if(size > 0)
		{
			cocktailSort(arr, size);// ����������
			cout << "��������������� ������: ";
		
			char i = 0;
			while(i < size)
			{
				cout << (int)arr[i++] << ' ';
			}
			cout << endl;
			size = 0;
		}
		cin.ignore(INT_MAX, '\n');
	}
}