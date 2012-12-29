#ifndef CUBE_H
#define CUBE_H

#include "mesh.h"

class Cube : public Mesh
{
public:
    Cube(QVector3D center, double size);
    void Draw(MeshDrawer &drawer);

private:
    QVector3D m_center;
    double m_size;
};

#endif // CUBE_H
