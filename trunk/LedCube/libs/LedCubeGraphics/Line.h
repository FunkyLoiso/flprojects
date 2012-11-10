#ifndef LINE_H
#define LINE_H

#include "../LedCubeBasic/LedCubeData.h"
#include "Point.h"

class Line3d
{
public:
	Line3d();
	Line3d(const Point3d& beg, const Point3d& end);
	void setBeg(const Point3d& beg);
	void setEnd(const Point3d& end);
	
	void render(LedCubeData& data) const;

private:
	Point3d m_beg, m_end;
};

#endif LINE_H