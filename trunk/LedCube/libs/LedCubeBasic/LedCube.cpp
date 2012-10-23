LedCube::LedCube(int latchPin, int clockPin, int dataPin)
: m_latchPin(latchPin), m_clockPin(clockPin), m_dataPin(dataPin)
{
  pinMode(m_latchPin, OUTPUT);
  pinMode(m_clockPin, OUTPUT);
  pinMode(m_dataPin, OUTPUT);
}
 
void LedCube::lightOne(byte x, byte y, byte z)
{
  int curX = 7;
  digitalWrite(m_latchPin, LOW);
   
  shiftOut(m_dataPin, m_clockPin, MSBFIRST, 1 << z);//layer = z
  for(; curX > x; --curX) shiftOut(m_dataPin, m_clockPin, MSBFIRST, 0);// lines after x
  shiftOut(m_dataPin, m_clockPin, MSBFIRST, 1 << y);// line x, led y
  --curX;
  for(; curX >= 0; --curX) shiftOut(m_dataPin, m_clockPin, MSBFIRST, 0);// lines before x
 
  digitalWrite(m_latchPin, HIGH);
}
 
void LedCube::lightLevel(byte z, byte data[8])
{
  digitalWrite(m_latchPin, LOW);
   
  shiftOut(m_dataPin, m_clockPin, MSBFIRST, 1 << z);//layer = z
  for(int i = 7; i >= 0; --i) shiftOut(m_dataPin, m_clockPin, MSBFIRST, data[i]);
 
  digitalWrite(m_latchPin, HIGH);
}
 
void LedCube::lightCube(byte data[8][8])
{
  for(int i = 0; i <= 7; ++i) lightLevel(i, data[i]);
}
 
void LedCube::lightCube(LedCubeData& data)
{
  lightCube(data.m_data);
}