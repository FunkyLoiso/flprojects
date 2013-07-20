#ifndef CLOCK_H
#define CLOCK_H

#include <windows.h>

/* Класс для синхронизации с абсолютной шкалой времени. Должен говорить, сколько времени
 * прошло с начала отсчёта, а так же обладать способностью ставиться на паузу*/
class Clock
{
public:
	Clock();
	void Set(int time_ms);	//Установить начальное значение
	void Go();				//Начать отсчёт
	void Pause();			//Приостановить отсчёт
	int Get_ms();			//Получить текущее значение на часах
	
private:
	LARGE_INTEGER set, lastPause, last, freq;
	int set_ms;
	bool paused;

	int getDiffFromNow_ms(LARGE_INTEGER last);
};

#endif /*CLOCK_H*/
