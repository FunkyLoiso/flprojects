#ifndef POINT_H
#define POINT_H

class Point3d
{
public:
	Point3d()
	{
		memset(m_data, 0, 3*sizeof(byte));
	}

	Point3d(byte x, byte y, byte z)
	{
		m_data[0] = x;
		m_data[1] = y;
		m_data[2] = z;
	}
	
	Point3d(const Point3d& other)
	{
		*this = other;
	}
	
	Point3d& operator=(const Point3d& other)
	{
		memcpy(m_data, other.m_data, 3*sizeof(byte));
	}
	
	byte& x() {return m_data[0];}
	byte x() const {return m_data[0];}
	byte& y() {return m_data[1];}
	byte y() const {return m_data[1];}
	byte& z() {return m_data[2];}
	byte z() const {return m_data[2];}
	
	byte& operator[](byte idx)
	{
		return m_data[idx];
	}
	
private:
	byte m_data[3];
};

#endif POINT_H