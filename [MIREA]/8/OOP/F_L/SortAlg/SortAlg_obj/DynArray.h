#ifndef DynArray_h__
#define DynArray_h__

class DynArray
{
public:
	DynArray(void);
	~DynArray(void);

	void add(char value);// �������� �������
	int size();//������� ������� ������ �������
	char at(char idx);//�������� �������� �� �������
	char operator[](char idx);//�������� �������� �� �������
	void clear();//������� ��� ��������

	void sort();

private:
	char* m_arr;
	char m_size;
	unsigned char m_allocSize;
};
#endif // DynArray_h__
