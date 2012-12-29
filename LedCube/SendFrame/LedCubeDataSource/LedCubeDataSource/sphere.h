#ifndef SPHERE_H
#define SPHERE_H

#include "Mesh.h"

class Sphere : public Mesh
{
public:
    Sphere(float radius, int slices, int stacks);

    void Draw(MeshDrawer &drawer);

private:
    float m_rad;
    int m_slices;
    int m_stacks;
};

#endif // SPHERE_H
