#ifndef LED_CUBE_H
#define LED_CUBE_H

#include <Arduino.h> 
#include "LedCubeData.h"

/*
 *	Диодный куб 8x8x8.
 *	Все размерности нумеруются с ноля. Этажи нумеруются по z, линии по x, диоды в линии по y
 *	Любая функция зажигает переданные диоды и гасит все остальные.
 *	Диоды в линии нумеруются с младщего бита, то есть 1 << 2 - второй диод (y = 2)
 */
class LedCube
{
public:
  LedCube(byte latchPin, byte clockPin, byte dataPin);	//Все пины - цифроыве, PWM не используется
  void lightOne(byte x, byte y, byte z);	//зажечь один диод
  void lightLevel(byte z, byte data[8]);	//зажечь уровень на основе массива линий
  void lightCube(byte data[8][8]);			//зажечь куб на основе массива уровней
  void lightCube(LedCubeData& data);		//зажечь куб на основе LedCubeData
  
  void turnOut();							//погасить куб
 
private:
  byte m_latchPin, m_clockPin, m_dataPin;
};

#endif LED_CUBE_H