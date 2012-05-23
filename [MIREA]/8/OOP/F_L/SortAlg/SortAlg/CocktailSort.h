#include <utility>

void cocktailSort( char* arr, char size )
{
	char first = -1;		//������ ������ ��� ����������
	char last = size - 2;	//��������� ������ ��� ����������
	bool swapped = true;
	while(swapped)
	{
		swapped = false;
		++first;	//����������� first, ��� ��� ��� �������� ����� ��� ��� �������������
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
		--last;	//��������� last, ��� ��� ��� �������� ����� ���� ��� �������������
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