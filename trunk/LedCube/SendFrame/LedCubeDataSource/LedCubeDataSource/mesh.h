#ifndef MESH_H
#define MESH_H

#include "meshdrawer.h"

class Mesh
{
public:
    virtual void Draw(MeshDrawer& drawer) = 0;
};

#endif // MESH_H
