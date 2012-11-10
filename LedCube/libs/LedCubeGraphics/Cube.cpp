#include "Cube.h"

Cube::Cube()
: m_size(0)
{}

Cube::Cube(const Point3d& origin, byte size)
: m_origin(origin), m_size(size)
{}
	
void Cube::setOrigin(const Point3d& origin)
{
	m_origin = origin;
}
void Cube::setSize(byte size)
{
	m_size = size;
}

void drawSquareZ(Point3d origin, byte size, LedCubeData& data)
{
	Point3d dst = origin; dst.x() += size;
	Line3d(origin, dst).render(data);
	origin = dst; origin.y() += size;
	Line3d(dst, origin).render(data);
	dst = origin; dst.x() -= size;
	Line3d(origin, dst).render(data);
	origin = dst; origin.y() -= size;
	Line3d(dst, origin).render(data);
}
	
void Cube::render(LedCubeData& data) const
{
	Point3d p1 = m_origin, p2 = m_origin;
	drawSquareZ(p1, m_size, data);
	p2.z() += m_size;
	drawSquareZ(p2, m_size, data);
	
	Line3d(p1, p2).render(data);
	p1.x() += m_size; p2.x() += m_size;
	Line3d(p1, p2).render(data);
	p1.y() += m_size; p2.y() += m_size;
	Line3d(p1, p2).render(data);
	p1.x() -= m_size; p2.x() -= m_size;
	Line3d(p1, p2).render(data);
}