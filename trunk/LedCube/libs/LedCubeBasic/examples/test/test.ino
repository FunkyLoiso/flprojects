#include <LedCube.h>
#include <LedCubeData.h>

const byte latchPin = 8;
const byte clockPin = 12;
const byte dataPin = 11;
 
LedCube cube(latchPin, clockPin, dataPin);
LedCubeData data;

void setup()
{

}

void loop()
{
  cube.lightOne(2, 3, 4);//зажечь диод в кубе
  
  bitWrite(data[5][3], 5, true);//установить один из диодов в data
  
  cube.lightCube(data);  // засветить на основе data
  cube.turnOut();        // погасить куб
  
  LedCubeData other;
  other = data;
  LedCubeData yetAnother(other); //разное копирование
}
