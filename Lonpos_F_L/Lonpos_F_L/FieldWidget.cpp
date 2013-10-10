#include "FieldWidget.h"

#include <QMouseEvent>
#include <QResizeEvent>
#include <QPainter>

FieldWidget::FieldWidget(QWidget *parent)
: QWidget(parent)
, m_fieldConf(0), m_figures(0)
, m_fieldWidth(0), m_fieldHeight(0)
, m_layoutMode(MODE_FIT)
{
}

FieldWidget::~FieldWidget()
{

}

void FieldWidget::setFieldWidth(int width)
{
	m_fieldWidth = width;
}

int FieldWidget::fieldWidth() const
{
	return m_fieldWidth;
}

void FieldWidget::setFieldHeight(int height)
{
	m_fieldHeight = height;
}

int FieldWidget::fieldHeight() const
{
	return m_fieldHeight;
}

void FieldWidget::setLayoutMode(LayoutMode mode)
{
	m_layoutMode = mode;
}

void FieldWidget::setFigures(Figure::list* figures)
{
	m_figures = figures;
}

void FieldWidget::setFieldConfiguration(FieldPlace::list* fieldConfiguration)
{
	m_fieldConf = fieldConfiguration;
}

void FieldWidget::setOverlayFigure(const Figure& overlayFigure)
{
	m_overlayFigure = overlayFigure;
}

void FieldWidget::removeOverlayFigure()
{
	m_overlayFigure = Figure::InvalidFigure();
}

void FieldWidget::setTrackingFigure(const Figure& trackingFigure)
{
	m_trackingFigure = trackingFigure;
}

void FieldWidget::removeTrackingFigure()
{
	m_trackingFigure = Figure::InvalidFigure();
}

void FieldWidget::mousePressEvent(QMouseEvent *e)
{
	if(e->button() == Qt::RightButton) emit rmbClicked();
	else if(e->button() == Qt::LeftButton)
	{

	}
}

void FieldWidget::mouseMoveEvent(QMouseEvent *)
{
}

void FieldWidget::keyPressEvent(QKeyEvent *)
{
}

void FieldWidget::paintEvent(QPaintEvent *)
{
	double cellSize = 0;	//размер €чейки
	switch(m_layoutMode)
	{
	case MODE_VERTICAL:
		cellSize = double(width())/m_fieldWidth;
		break;
	case MODE_FIT:
		cellSize = qMin(double(width())/m_fieldWidth, double(height())/m_fieldHeight);
		break;
	}

	QPainter p(this);
	p.setRenderHint(QPainter::Antialiasing);
	p.fillRect(rect(), Qt::white);

	if(m_fieldConf != NULL)
	{
		p.setPen(Qt::NoPen);
		p.setBrush(Qt::lightGray);
		Q_FOREACH(FieldPlace place, *m_fieldConf)
		{
			QRectF circleRect(place.x()*cellSize, place.y()*cellSize, cellSize, cellSize);
			p.drawEllipse(circleRect);
		}
	}

	p.setPen(Qt::black);
	if(m_figures != NULL)
	{
		Q_FOREACH(const Figure& figure, *m_figures)
		{
			p.setBrush(figure.color());
			Q_FOREACH(FieldPlace place, figure.places())
			{
				QRectF circleRect(place.x()*cellSize, place.y()*cellSize, cellSize, cellSize);
				p.drawEllipse(circleRect);
			}
		}
	}
}

QSize FieldWidget::sizeHint() const
{
	if(m_layoutMode == MODE_VERTICAL)
	{
		double cellSize = double(width())/m_fieldWidth;
		return QSize(0, cellSize * m_fieldHeight);
	}
	else return QSize();
}
