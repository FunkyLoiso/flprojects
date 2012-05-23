#include <signal.h>
#include <iostream>
using namespace std;

#include "CocktailSort.h"

static const char INIT_SIZE = 10;
static const char k = 2;

// процедура добавления элемента в массив
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
	unsigned char allocSize = INIT_SIZE;	//Выделенное пространство
	char size = 0;							//Число элементов в массиве
	char* arr = (char*)malloc(allocSize * sizeof(char));

	setlocale(LC_ALL, "Russian");//для вывода латиницы
	cout << "Сортировка одномерного динамического массива по возрастанию. Филон Д.В.ВСМ-06-08" << endl;
	
	while(true)
	{
		while(true)
		{
			cout << endl << "Введите целые числа["<< SCHAR_MIN << ", " << SCHAR_MAX << "] через пробел. Enter для завершения, q для выхода" << endl;
			cout << ">";

			short tmp;
			while(std::cin.good() && std::cin.peek() != '\n')
			{
				std::cin >> tmp;

				if(std::cin.good())
				{
					if(tmp > SCHAR_MAX || tmp < SCHAR_MIN)
					{//если вышли за пределы диапазона значений - игнорируем
						cout << "Элемент " << tmp << " выходит за пределы допустимого диапазона ["<< SCHAR_MIN << ", " << SCHAR_MAX << "] и будет проигнорирован!" << endl;
						continue;
					}
					insert(tmp, arr, size, allocSize);
				}

				if(!std::cin.good())
				{
					std::cin.clear();

					if(cin.peek() == 'q' || cin.peek() == 'Q')
					{//выход из программы
						free(arr);
						cout << endl << "До свидания!" << endl;
						return 0;
					}
					//пропуск недопустимых подстрок
					cout << "Обнаружены недопустимые символы, подстрока ";
					while(std::cin.peek() != ' ' && std::cin.peek() != '\n') cout << (char)std::cin.get();
					cout << " будет проигнорирована" << endl;
				}
				
				if(size >= SCHAR_MAX)
				{// если уже введено 127 элементов
					cout << endl << "Введено максимальное количество элементов, остальные будут проигнорированы. Начинается сортировка!" << endl;
					break;
				}
			}

			if(size == 0)
			{
				cout << "Не введено ни одного элемента" << endl;
				break;
			}

			break;
		}
		// вывод отсортированного массива на экран
		if(size > 0)
		{
			cocktailSort(arr, size);// сортировка
			cout << "Отсортированный массив: ";
		
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