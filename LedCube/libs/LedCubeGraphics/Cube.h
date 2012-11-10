#ifndef CUBE_H
#define CUBE_H

#include "Line.h"

class Cube
{
public:
	Cube();
	Cube(const Point3d& origin, byte size);
	
	void setOrigin(const Point3d& origin);
	void setSize(byte size);
	
	void render(LedCubeData& data) const;
	
private:
	Point3d m_origin;
	byte m_size;
};

#endif CUBE_H