#ifndef LED_CUBE_DATA_H
#define LED_CUBE_DATA_H

#include <Arduino.h> 

/*
 *	Полная информация о диодах в кубе 8x8x8.
 *	Значения не инициализируются в конструкторе!
 */
class LedCubeData
{
public:
	//установка значений
	void setLed(byte x, byte y, byte z, bool on);	//изменить состояние диода
	void setLevel(byte z, byte data[8]);			//изменить уровень
	void setData(byte data[8][8]);					//изменить весь куб
	
	void clear(byte value = 0);						//заполнить все линии заданным значением

	//мягкий доступ
	bool isLedSet(byte x, byte y, byte z) const;	//получить значение диода
	byte line(byte x, byte z) const;				//получить значение линии
	
	//жёсткий доступ
	byte* level(byte z);							//получить указатель на уровень
	byte* operator[](byte z);						//получить указатель на уровень
	byte& line(byte x, byte z);						//получить значение линии
 
private:
	friend class LedCube;	// для прямого доступа к m_data
	byte m_data[8][8];		// (z; x)
};

#endif LED_CUBE_DATA_H