#include "LedCubeData.h"

LedCubeData::LedCubeData()
{}

LedCubeData& LedCubeData::operator=(const LedCubeData& other)
{
	memcpy(m_data, other.m_data, 8*8*sizeof(byte));
}

LedCubeData::LedCubeData(const LedCubeData& other)
{
	*this = other;
}

//��������� ��������
void LedCubeData::setLed(byte x, byte y, byte z, bool on)
{
	bitWrite(m_data[z][x], y, on);
}
 
void LedCubeData::setLevel(byte z, byte data[8])
{
	memcpy(m_data[z], data, 8*sizeof(byte));
}
 
void LedCubeData::setData(byte data[8][8])
{
	memcpy(m_data, data, 8*8*sizeof(byte));
}

void LedCubeData::clear(byte value)
{
	memset(m_data, 0, 8*8*sizeof(byte));
}

void LedCubeData::clearLevel(byte level, byte value)
{
	memset(this->level(level), value, 8*sizeof(byte));
}

//��������� ��������
bool LedCubeData::isLedSet(byte x, byte y, byte z) const
{
	return bitRead(m_data[z][x], y);
}

byte LedCubeData::line(byte x, byte z) const
{
	return m_data[z][x];
}

//������ ������
byte* LedCubeData::level(byte z)
{
	return m_data[z];
}

byte* LedCubeData::operator[](byte z)
{
	return level(z);
}

byte& LedCubeData::line(byte x, byte z)
{
	return  m_data[z][x];
}

