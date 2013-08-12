#ifndef GLASSWIDGET_H
#define GLASSWIDGET_H

#include <QWidget>

class Glass;

class GlassWidget : public QWidget
{
	Q_OBJECT

public:
	GlassWidget(QWidget *parent = 0);
	~GlassWidget();

	void setGlass(Glass* glass);

private:
	Glass* m_glass;

	virtual void mousePressEvent(QMouseEvent *);
	virtual void mouseReleaseEvent(QMouseEvent *);
	virtual void mouseMoveEvent(QMouseEvent *);
	virtual void paintEvent(QPaintEvent *);
};

#endif // GLASSWIDGET_H
