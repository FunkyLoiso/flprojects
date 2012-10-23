#include "LedCube.h"

LedCube::LedCube(byte latchPin, byte clockPin, byte dataPin)
	: m_latchPin(latchPin), m_clockPin(clockPin), m_dataPin(dataPin)
{
	pinMode(m_latchPin, OUTPUT);	//все пины посылают сигналы
	pinMode(m_clockPin, OUTPUT);
	pinMode(m_dataPin, OUTPUT);
}
 
void LedCube::lightOne(byte x, byte y, byte z)
{
	int curX = 7;//текущая линия
	digitalWrite(m_latchPin, LOW);	//теперь изменения в регистрах не отразятся на кубе

	//регистры наполняются с последнего, сначала регистр уровня, потом 7-я линия, 6-я и так далее
	shiftOut(m_dataPin, m_clockPin, MSBFIRST, 1 << z);//задействуем уровень z
	for(; curX > x; --curX) shiftOut(m_dataPin, m_clockPin, MSBFIRST, 0);//пропускаем все линии с 7ой до x-овой
	shiftOut(m_dataPin, m_clockPin, MSBFIRST, 1 << y);//зажигаем на x-овой линии диод y
	--curX;
	for(; curX >= 0; --curX) shiftOut(m_dataPin, m_clockPin, MSBFIRST, 0);//пропускаем все оставшиеся линии до нолевой

	digitalWrite(m_latchPin, HIGH);	//теперь изменения вступят в силу
}
 
void LedCube::lightLevel(byte z, byte data[8])
{
	digitalWrite(m_latchPin, LOW);	//теперь изменения в регистрах не отразятся на кубе

	shiftOut(m_dataPin, m_clockPin, MSBFIRST, 1 << z);//задействуем уровень z
	for(int i = 7; i >= 0; --i) shiftOut(m_dataPin, m_clockPin, MSBFIRST, data[i]);//линии передаются начиная с 7-ой

	digitalWrite(m_latchPin, HIGH);	//теперь изменения вступят в силу
}
 
void LedCube::lightCube(byte data[8][8])
{
	//зажгём поочерёдно все уровни начиная с 0-ого. В каждый отдельный момент может гореть только один!
	for(int i = 0; i <= 7; ++i) lightLevel(i, data[i]);
}
 
void LedCube::lightCube(LedCubeData& data)
{
  lightCube(data.m_data);//отобразим массив из LedCubeData
}

void LedCube::turnOut()
{
	digitalWrite(m_latchPin, LOW);	//теперь изменения в регистрах не отразятся на кубе
   
	shiftOut(m_dataPin, m_clockPin, MSBFIRST, 0);//выключить все уровни

	digitalWrite(m_latchPin, HIGH);	//теперь изменения вступят в 
}