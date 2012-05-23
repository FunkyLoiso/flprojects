#ifndef DynArray_h__
#define DynArray_h__

class DynArray
{
public:
	DynArray(void);
	~DynArray(void);

	void add(char value);// добавить элемент
	int size();//вернуть текущий размер массива
	char at(char idx);//получить значение по индексу
	char operator[](char idx);//получить значение по индексу
	void clear();//удалить все элементы

	void sort();

private:
	char* m_arr;
	char m_size;
	unsigned char m_allocSize;
};
#endif // DynArray_h__
