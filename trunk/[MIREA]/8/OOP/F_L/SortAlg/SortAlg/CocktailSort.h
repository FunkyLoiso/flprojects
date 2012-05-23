#include <utility>

void cocktailSort( char* arr, char size )
{
	char first = -1;		//первый индекс для сортировки
	char last = size - 2;	//последний индекс для сортировки
	bool swapped = true;
	while(swapped)
	{
		swapped = false;
		++first;	//увеличиваем first, так как все элементы перед ним уже отсортированы
		char i = first;
		while(i <= last)
		{
			if(arr[i] > arr[i+1])
			{
				std::swap(arr[i], arr[i+1]);
				swapped = true;
			}
			++i;
		}
		if(!swapped) break;

		swapped = false;
		--last;	//уменьшаем last, так как все элементы после него уже отсортированы
		i = last;
		while(i >= first)
		{
			if(arr[i] > arr[i+1])
			{
				std::swap(arr[i], arr[i+1]);
				swapped = true;
			}
			--i;
		}
	}
}