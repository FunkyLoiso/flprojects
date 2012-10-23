#ifndef LED_CUBE_H
#define LED_CUBE_H

#include <Arduino.h> 
#include "LedCubeData.h"

/*
 *	������� ��� 8x8x8.
 *	��� ����������� ���������� � ����. ����� ���������� �� z, ����� �� x, ����� � ����� �� y
 *	����� ������� �������� ���������� ����� � ����� ��� ���������.
 *	����� � ����� ���������� � �������� ����, �� ���� 1 << 2 - ������ ���� (y = 2)
 */
class LedCube
{
public:
  LedCube(byte latchPin, byte clockPin, byte dataPin);	//��� ���� - ��������, PWM �� ������������
  void lightOne(byte x, byte y, byte z);	//������ ���� ����
  void lightLevel(byte z, byte data[8]);	//������ ������� �� ������ ������� �����
  void lightCube(byte data[8][8]);			//������ ��� �� ������ ������� �������
  void lightCube(LedCubeData& data);		//������ ��� �� ������ LedCubeData
  
  void turnOut();							//�������� ���
 
private:
  byte m_latchPin, m_clockPin, m_dataPin;
};

#endif LED_CUBE_H