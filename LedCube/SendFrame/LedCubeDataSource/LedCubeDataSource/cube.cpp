#include "cube.h"

Cube::Cube(QVector3D center, double size)
    : m_center(center), m_size(size)
{
}

void Cube::Draw(MeshDrawer &drawer)
{
    double x = m_center.x();
    double y = m_center.y();
    double z = m_center.z();
    double size = m_size/2;

    QVector3D g1(x-size, y-size, z-size);
    QVector3D g2 = g1 + QVector3D(2*size, 0.0, 0.0);
    QVector3D g3 = g2 + QVector3D(0.0, 2*size, 0.0);
    QVector3D g4 = g3 - QVector3D(2*size, 0.0, 0.0);

    QVector3D r1(x-size, y-size, z+size);
    QVector3D r2 = r1 + QVector3D(2*size, 0.0, 0.0);
    QVector3D r3 = r2 + QVector3D(0.0, 2*size, 0.0);
    QVector3D r4 = r3 - QVector3D(2*size, 0.0, 0.0);

    drawer.drawLine(g1, g2);
    drawer.drawLine(g2, g3);
    drawer.drawLine(g3, g4);
    drawer.drawLine(g4, g1);

    drawer.drawLine(r1, r2);
    drawer.drawLine(r2, r3);
    drawer.drawLine(r3, r4);
    drawer.drawLine(r4, r1);

    drawer.drawLine(g1, r1);
    drawer.drawLine(g2, r2);
    drawer.drawLine(g3, r3);
    drawer.drawLine(g4, r4);
}
