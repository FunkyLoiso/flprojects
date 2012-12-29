#include "sphere.h"
#include "qmath.h"

Sphere::Sphere(float radius, int slices, int stacks)
    : m_rad(radius), m_slices(slices), m_stacks(stacks)
{
}

void Sphere::Draw(MeshDrawer &drawer)
{
    typedef float GLfloat;
    typedef int GLint;
    GLfloat drho = (GLfloat)(3.141592653589) / (GLfloat) m_stacks;
    GLfloat dtheta = 2.0f * (GLfloat)(3.141592653589) / (GLfloat) m_slices;
    GLfloat ds = 1.0f / (GLfloat) m_slices;
    GLfloat dt = 1.0f / (GLfloat) m_stacks;
    GLfloat t = 1.0f;
    GLfloat s = 0.0f;
    GLint i, j;     // Looping variables

    for (i = 0; i < m_stacks; i++)
        {
        GLfloat rho = (GLfloat)i * drho;
        GLfloat srho = (GLfloat)(qSin(rho));
        GLfloat crho = (GLfloat)(cos(rho));
        GLfloat srhodrho = (GLfloat)(qSin(rho + drho));
        GLfloat crhodrho = (GLfloat)(cos(rho + drho));

        // Many sources of OpenGL sphere drawing code uses a triangle fan
        // for the caps of the sphere. This however introduces texturing
        // artifacts at the poles on some OpenGL implementations
        s = 0.0f;
        for ( j = 0; j <= m_slices; j++)
            {
            GLfloat theta = (j == m_slices) ? 0.0f : j * dtheta;
            GLfloat stheta = (GLfloat)(-sin(theta));
            GLfloat ctheta = (GLfloat)(cos(theta));

            GLfloat x = stheta * srho;
            GLfloat y = ctheta * srho;
            GLfloat z = crho;

            //glTexCoord2f(s, t);
//			glMultiTexCoord2fARB(GL_TEXTURE0_ARB, s, t);
//            glNormal3f(x, y, z);
            drawer.drawPoint(QVector3D(x * m_rad, y * m_rad, z * m_rad));

            x = stheta * srhodrho;
            y = ctheta * srhodrho;
            z = crhodrho;
            //glTexCoord2f(s, t - dt);
//			glMultiTexCoord2fARB(GL_TEXTURE0_ARB, s, t - dt);
            s += ds;
//            glNormal3f(x, y, z);
//            glVertex3f(x * m_rad, y * m_rad, z * m_rad);
            drawer.drawPoint(QVector3D(x * m_rad, y * m_rad, z * m_rad));
            }

        t -= dt;
        }
}
