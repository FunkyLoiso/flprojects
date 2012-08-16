#ifndef QBWidget_h__
#define QBWidget_h__

#include <QGLWidget>

class QBWidget : public QGLWidget
{
public:
	QBWidget(QWidget* parent = NULL);
	~QBWidget(void);

	void setSize(int x, int y, int z);
	void setState(int x, int y, int z, bool isOn);

protected:
	void initializeGL();
	void resizeGL(int width, int height);
	void paintGL();

private:
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void setXRotation(int angle);
	void setYRotation(int angle);
	void setZRotation(int angle);

	void gltDrawSphere(GLfloat fRadius, GLint iSlices, GLint iStacks);

	int m_xRot;
	int m_yRot;
	int m_zRot;
	QPoint m_lastPos;

	QColor m_colorBackground;
	GLfloat m_onColor[4];
	GLfloat m_offColor[4];

	QVector< QVector< QVector<bool> > > m_leds;
};

#endif // QBWidget_h__