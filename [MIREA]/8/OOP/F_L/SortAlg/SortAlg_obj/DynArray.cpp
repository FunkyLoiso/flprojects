#include "DynArray.h"
#include "utility"
#include "stdlib.h"

static const char INIT_SIZE = 10;
static const char k = 2;

DynArray::DynArray(void)
{
	m_allocSize = INIT_SIZE;	//Выделенное пространство
	m_size = 0;					//Число элементов в массиве
	m_arr = (char*)malloc(m_allocSize * sizeof(char));
}

DynArray::~DynArray(void)
{
	delete[] m_arr;
}

void DynArray::add( char value )
{
	if(m_size >= m_allocSize)
	{
		m_allocSize += m_allocSize/k;
		m_arr = (char*)realloc(m_arr, m_allocSize * sizeof(char));
	}
	m_arr[m_size++] = value;
}

int DynArray::size()
{
	return m_size;
}

char DynArray::at( char idx )
{
	return m_arr[idx];
}

char DynArray::operator[]( char idx )
{
	return at(idx);
}

void DynArray::clear()
{
	m_size = 0;
}


void DynArray::sort()
{
	char first = -1;
	char last = m_size - 2;
	bool swapped = true;
	while(swapped)
	{
		swapped = false;
		++first;	//увеличиваем first, так как все элементы перед ним уже отсортированы
		char i = first;
		while(i <= last)
		{
			if(m_arr[i] > m_arr[i+1])
			{
				std::swap(m_arr[i], m_arr[i+1]);
				swapped = true;
			}
		}
		if(!swapped) break;

		swapped = false;
		--last;	//уменьшаем last, так как все элементы после него уже отсортированы
		i = last;
		while(i >= first)
		{
			if(m_arr[i] > m_arr[i+1])
			{
				std::swap(m_arr[i], m_arr[i+1]);
				swapped = true;
			}
		}
	}
}
