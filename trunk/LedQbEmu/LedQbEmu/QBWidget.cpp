#include "QBWidget.h"

#include <QtGui>
#include <QtOpenGL>
#include <QElapsedTimer>

#include <math.h>

#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE  0x809D
#endif

static void normalizeAngle(int &angle)
{
	while (angle < 0)
		angle += 360 * 16;
	while (angle > 360 * 16)
		angle -= 360 * 16;
}


QBWidget::QBWidget(int width, int depth, int height, QWidget* parent)
:	QGLWidget(parent), m_xRot(0), m_yRot(0), m_zRot(0), m_leds(NULL)
{
	setSize(width, depth, height);
}

QBWidget::QBWidget(int width, int depth, int height, const QGLFormat &format, QWidget* parent)
:	QGLWidget(format, parent), m_xRot(0), m_yRot(0), m_zRot(0), m_leds(NULL)
{
	setSize(width, depth, height);
}

QBWidget::~QBWidget(void)
{
}

void QBWidget::initializeGL()
{
	m_colorBackground = Qt::gray;

	m_onColor[0] = 0;
	m_onColor[1] = 1;
	m_onColor[2] = 0;
	m_onColor[3] = 1;

	m_offColor[0] = 0;
	m_offColor[1] = 0.2;
	m_offColor[2] = 0;
	m_offColor[3] = 1;

	m_groundColor[0] = 0.7;
	m_groundColor[1] = 0.7;
	m_groundColor[2] = 0.7;
	m_groundColor[3] = 1;

	qglClearColor(m_colorBackground);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_MULTISAMPLE);
	static GLfloat lightPosition[4] = { 0.5, 5.0, 7.0, 1.0 };
	//static GLfloat lightPosition[4] = { 0.0, 0.0, 0.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

	//список отображения для сферы
	m_sphereList = glGenLists(1);

	GLfloat width = m_leds.size();
	GLfloat depth = m_leds[0].size();
	GLfloat height = m_leds[0][0].size();
	GLfloat step = 10.0f / max(max(width, depth), height);

	glNewList(m_sphereList, GL_COMPILE);
	{
		drawSphere(step/10, 8, 4);
	}
	glEndList();
}

void QBWidget::resizeGL(int width, int height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//glOrtho(-0.5, +0.5, -0.5, +0.5, 4.0, 15.0);
	//void glFrustum(	GLdouble  	left, 
	//	GLdouble  	right, 
	//	GLdouble  	bottom, 
	//	GLdouble  	top, 
	//	GLdouble  	nearVal, 
	//	GLdouble  	farVal);
	//glFrustum(-1.5, +1.5, -1.5, +1.5, 4.0, 15.0);

	GLfloat zNear = 0.1f;
	GLfloat zFar = 300.0f;
	GLfloat fieldOfView = 60.0;

	GLfloat aspect = float(width)/float(height);
	GLfloat fH = tan( float(fieldOfView / 360.0f * 3.14159f) ) * zNear;
	GLfloat fW = fH * aspect;
	glFrustum( -fW, fW, -fH, fH, zNear, zFar );

	glMatrixMode(GL_MODELVIEW);
}

void QBWidget::paintGL()
{
	static QElapsedTimer timer;
	static int counter = 0;
	static const int maxCount = 10;

	if(++counter == maxCount)
	{
		m_fps = float(maxCount) / timer.elapsed() * 1000;
		counter = 0;
		timer.restart();
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	//translate so we look towards center from (0, -15, 0)
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	glTranslatef(0.0, 16.0, 0.0);

	glRotatef(m_xRot / 16.0, 1.0, 0.0, 0.0);
	glRotatef(m_yRot / 16.0, 0.0, 1.0, 0.0);
	glRotatef(m_zRot / 16.0, 0.0, 0.0, 1.0);

	drawGround();
	drawLeds();

	QTimer::singleShot(1, this, SLOT(updateGL()));
}

void QBWidget::mousePressEvent(QMouseEvent *event)
{
	m_lastPos = event->pos();
}

void QBWidget::mouseMoveEvent(QMouseEvent *event)
{
	int dx = event->x() - m_lastPos.x();
	int dy = event->y() - m_lastPos.y();

	if (event->buttons() & Qt::LeftButton)
	{
		setXRotation(m_xRot + 8 * dy);
		setZRotation(m_zRot + 8 * dx);
	}
	else if (event->buttons() & Qt::RightButton)
	{
		setXRotation(m_xRot + 8 * dy);
		setYRotation(m_yRot + 8 * dx);
	}
	m_lastPos = event->pos();
}

void QBWidget::setXRotation(int angle)
{
	normalizeAngle(angle);
	if (angle != m_xRot)
	{
		m_xRot = angle;
		//updateGL();
	}
}

void QBWidget::setYRotation(int angle)
{
	normalizeAngle(angle);
	if (angle != m_yRot)
	{
		m_yRot = angle;
		//updateGL();
	}
}

void QBWidget::setZRotation(int angle)
{
	normalizeAngle(angle);
	if (angle != m_zRot)
	{
		m_zRot = angle;
		//updateGL();
	}
}

void QBWidget::setSize(int x, int y, int z)
{
	m_leds.resize(x);
	for(int cx = 0; cx != x; ++cx)
	{
		m_leds[cx].resize(y);
		for(int cy = 0; cy != y; ++cy)
		{
			m_leds[cx][cy].resize(z);
		}
	}
	//updateGL();
}

void QBWidget::setState(int x, int y, int z, bool isOn)
{
	m_leds[x][y][z] = isOn;
	//updateGL();
}

void QBWidget::drawGround()
{
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, m_groundColor);

	GLfloat side = 40;//длина стороны
	GLfloat texSide = 1;//Размер стороны одной "плитки"
	GLfloat y = -5.4f;//где по y лежит
	/*рисуем сам квадратик земли*/
	glBegin(GL_QUADS);
	{
		glNormal3f(0, 0, 1);

		glTexCoord2f(0, 0);
		glVertex3f(-side/2,side/2, y);
		glTexCoord2f(0, side/texSide);
		glVertex3f(-side/2, -side/2, y);
		glTexCoord2f(side/texSide, side/texSide);
		glVertex3f(side/2, -side/2, y);
		glTexCoord2f(side/texSide, 0);
		glVertex3f(side/2, side/2, y);





	}
	glEnd();
}

void QBWidget::drawLeds()
{
	GLfloat width = m_leds.size();
	GLfloat depth = m_leds[0].size();
	GLfloat height = m_leds[0][0].size();

	GLfloat step = 10.0f / max(max(width, depth), height);

	glTranslatef(-width/2*step, -depth/2*step, -height/2*step);

	for(int cx = 0; cx != width; ++cx)
	{
		for(int cy = 0; cy != depth; ++cy)
		{
			for(int cz = 0; cz != height; ++cz)
			{
				glPushMatrix();

				glTranslatef(cx * step, cy * step, cz * step);
				glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, m_leds[cx][cy][cz] ? m_onColor : m_offColor);
				//drawSphere(step/10, 4, 2);
				glCallList(m_sphereList);

				glPopMatrix();
			}
		}
	}
}

void QBWidget::drawSphere(GLfloat fRadius, GLint iSlices, GLint iStacks)
{
	GLfloat drho = (GLfloat)(3.141592653589) / (GLfloat) iStacks;
	GLfloat dtheta = 2.0f * (GLfloat)(3.141592653589) / (GLfloat) iSlices;
	GLfloat ds = 1.0f / (GLfloat) iSlices;
	GLfloat dt = 1.0f / (GLfloat) iStacks;
	GLfloat t = 1.0f;	
	GLfloat s = 0.0f;
	GLint i, j;     // Looping variables

	for (i = 0; i < iStacks; i++) 
	{
		GLfloat rho = (GLfloat)i * drho;
		GLfloat srho = (GLfloat)(sin(rho));
		GLfloat crho = (GLfloat)(cos(rho));
		GLfloat srhodrho = (GLfloat)(sin(rho + drho));
		GLfloat crhodrho = (GLfloat)(cos(rho + drho));

		// Many sources of OpenGL sphere drawing code uses a triangle fan
		// for the caps of the sphere. This however introduces texturing 
		// artifacts at the poles on some OpenGL implementations
		glBegin(GL_TRIANGLE_STRIP);
		s = 0.0f;
		for ( j = 0; j <= iSlices; j++) 
		{
			GLfloat theta = (j == iSlices) ? 0.0f : j * dtheta;
			GLfloat stheta = (GLfloat)(-sin(theta));
			GLfloat ctheta = (GLfloat)(cos(theta));

			GLfloat x = stheta * srho;
			GLfloat y = ctheta * srho;
			GLfloat z = crho;

			glTexCoord2f(s, t);
			glNormal3f(x, y, z);
			glVertex3f(x * fRadius, y * fRadius, z * fRadius);

			x = stheta * srhodrho;
			y = ctheta * srhodrho;
			z = crhodrho;
			glTexCoord2f(s, t - dt);
			s += ds;
			glNormal3f(x, y, z);
			glVertex3f(x * fRadius, y * fRadius, z * fRadius);
		}
		glEnd();

		t -= dt;
	}
}
