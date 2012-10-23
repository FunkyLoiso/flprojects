class LedCube
{
public:
  LedCube(int latchPin, int clockPin, int dataPin);
  void lightOne(byte x, byte y, byte z);
  void lightLevel(byte z, byte data[8]);
  void lightCube(byte data[8][8]);
  void lightCube(LedCubeData& data);
 
private:
  int m_latchPin, m_clockPin, m_dataPin;
};