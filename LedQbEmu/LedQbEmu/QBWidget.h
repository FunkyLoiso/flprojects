#ifndef QBWidget_h__
#define QBWidget_h__

#include <QGLWidget>

class QBWidget : public QGLWidget
{
public:
	QBWidget(QWidget* parent = NULL);
	QBWidget(const QGLFormat &format, QWidget* parent = NULL);
	~QBWidget(void);

	void setSize(int x, int y, int z);
	void setState(int x, int y, int z, bool isOn);

	float fps() {return m_fps;}

protected:
	void initializeGL();
	void resizeGL(int width, int height);
	void paintGL();

	void drawLeds();

private:
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void setXRotation(int angle);
	void setYRotation(int angle);
	void setZRotation(int angle);

	void drawSphere(GLfloat fRadius, GLint iSlices, GLint iStacks);
	void drawGround();
	int m_xRot;
	int m_yRot;
	int m_zRot;
	QPoint m_lastPos;
	float m_fps;

	QColor m_colorBackground;
	GLfloat m_onColor[4];
	GLfloat m_offColor[4];
	GLfloat m_groundColor[4];

	QVector< QVector< QVector<bool> > > m_leds;
};

#endif // QBWidget_h__
