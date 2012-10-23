#ifndef LED_CUBE_DATA_H
#define LED_CUBE_DATA_H

#include <Arduino.h> 

/*
 *	������ ���������� � ������ � ���� 8x8x8.
 *	�������� �� ���������������� � ������������!
 */
class LedCubeData
{
public:
	void setLed(byte x, byte y, byte z, bool on);	//�������� ��������� �����
	void setLevel(byte z, byte data[8]);			//�������� �������
	void setData(byte data[8][8]);					//�������� ���� ���

	bool isLedSet(byte x, byte y, byte z);			//�������� �������� �����
	byte* level(byte z);							//�������� ��������� �� �������
	byte line(byte y, byte z);						//�������� �������� �����

	void clear(byte value = 0);						//��������� ��� ����� �������� ���������
 
private:
	friend class LedCube;//��� ������� ������� � m_data
	byte m_data[8][8]; // (z; x)
};

#endif LED_CUBE_DATA_H