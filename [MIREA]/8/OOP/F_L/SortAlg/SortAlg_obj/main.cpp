#include <iostream>
using namespace std;

#include "DynArray.h"

int main()
{
	setlocale(LC_ALL, "Russian");//для вывода латиницы
	cout << "Сортировка одномерного динамического массива по возрастанию. Филон Д.В.ВСМ-06-08" << endl;

	while(true)
	{
		DynArray arr;
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
					arr.add(tmp);
				}

				if(!std::cin.good())
				{
					std::cin.clear();

					if(cin.peek() == 'q' || cin.peek() == 'Q')
					{//выход из программы
						cout << endl << "До свидания!" << endl;
						return 0;
					}
					//пропуск недопустимых подстрок
					cout << "Обнаружены недопустимые символы, подстрока ";
					while(std::cin.peek() != ' ' && std::cin.peek() != '\n') cout << (char)std::cin.get();
					cout << " будет проигнорирована" << endl;
				}

				if(arr.size() >= SCHAR_MAX)
				{// если уже введено 127 элементов
					cout << endl << "Введено максимальное количество элементов, остальные будут проигнорированы. Начинается сортировка!" << endl;
					break;
				}
			}

			if(arr.size() == 0)
			{
				cout << "Не введено ни одного элемента" << endl;
				break;
			}

			break;
		}
		// вывод отсортированного массива на экран
		if(arr.size() > 0)
		{
			arr.sort();// сортировка
			cout << "Отсортированный массив: ";

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