#include "glasswidget.h"
#include <QPainter>
#include "Glass.h"

GlassWidget::GlassWidget(QWidget *parent)
	: QWidget(parent), m_glass(NULL)
{
}

GlassWidget::~GlassWidget()
{

}

void GlassWidget::setGlass(Glass* glass)
{
	m_glass = glass;
	update();
}

void GlassWidget::paintEvent(QPaintEvent *)
{
	if(!m_glass) return;

	QPainter p(this);

	p.setRenderHint(QPainter::Antialiasing);

	QRectF boundingRect = m_glass->border.boundingRect();
	QRectF realRect = this->rect();
	realRect.adjust(0, 0, -1, -1);
	qreal ratio = qMin(realRect.width() / boundingRect.width(), realRect.height() / boundingRect.height());
	p.setTransform(QTransform::fromScale(ratio, ratio));

	p.setPen(Qt::red);
	p.drawPolygon(m_glass->border);

	p.setPen(Qt::blue);
	QColor particleColor(0, 0, 255, 128);
	p.setBrush(particleColor);

	for(Glass::TParticlesMap::ConstIterator i = m_glass->particles.constBegin(); i != m_glass->particles.constEnd(); ++i)
	{
		p.drawEllipse(i->pos.toPointF(), i->radius, i->radius);
	}
}

void GlassWidget::mousePressEvent(QMouseEvent *)
{
}

void GlassWidget::mouseReleaseEvent(QMouseEvent *)
{
}

void GlassWidget::mouseMoveEvent(QMouseEvent *)
{
}

