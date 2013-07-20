#ifndef CLOCK_H
#define CLOCK_H

#include <windows.h>

/* ����� ��� ������������� � ���������� ������ �������. ������ ��������, ������� �������
 * ������ � ������ �������, � ��� �� �������� ������������ ��������� �� �����*/
class Clock
{
public:
	Clock();
	void Set(int time_ms);	//���������� ��������� ��������
	void Go();				//������ ������
	void Pause();			//������������� ������
	int Get_ms();			//�������� ������� �������� �� �����
	
private:
	LARGE_INTEGER set, lastPause, last, freq;
	int set_ms;
	bool paused;

	int getDiffFromNow_ms(LARGE_INTEGER last);
};

#endif /*CLOCK_H*/
