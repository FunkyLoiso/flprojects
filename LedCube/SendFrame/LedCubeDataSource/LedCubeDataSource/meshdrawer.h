#ifndef MESHDRAWER_H
#define MESHDRAWER_H

#include <QMatrix4x4>
#include <QVector3D>

typedef unsigned char byte;

class MeshDrawer
{
public:
    MeshDrawer();

    void setMatrix(QMatrix4x4& matrix) {m_matrix = matrix;}
    QMatrix4x4 matrix() const {return m_matrix;}

    void setDest(byte*& dst) {m_dst = dst;}

    void drawPoint(QVector3D pt);
    void drawLine(QVector3D p1, QVector3D p2);

private:
    QMatrix4x4 m_matrix;
    byte* m_dst;

    void point3d(byte x, byte y, byte z);
    void line3d(int x1, int y1, int z1, int x2, int y2, int z2);
};

#endif // MESHDRAWER_H
