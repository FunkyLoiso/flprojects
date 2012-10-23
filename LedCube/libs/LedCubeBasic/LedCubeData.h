class LedCubeData
{
public:
  LedCubeData();
  void setLed(byte x, byte y, byte z, bool on);
  void setLevel(byte z, byte data[8]);
  void setData(byte data[8][8]);
 
  bool isLedSet(byte x, byte y, byte z);
  byte* level(byte z);
  byte line(byte y, byte z);
 
private:
  friend class LedCube;
  byte m_data[8][8]; // (z; y)
};