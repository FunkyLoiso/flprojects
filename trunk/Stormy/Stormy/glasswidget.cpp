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

	//QFont font;
	//font.setPixelSizeFloat(0.1);
	//p.setFont(font);
	//p.drawText(0.1, 0.1, "Cock!");

	p.setPen(Qt::red);
	p.drawPolygon(m_glass->border);

	for(Glass::TParticlesMap::ConstIterator i = m_glass->particles.constBegin(); i != m_glass->particles.constEnd(); ++i)
	{
		p.setPen(Qt::blue);
		p.setBrush(QColor(i->dbg_level%256, 0, 255, 128));
		p.drawEllipse(i->pos.toPointF(), i->radius, i->radius);

#ifndef NDEBUG
		p.setPen(Qt::black);
		static double k = 0.2;
		p.drawLine(i->pos.toPointF(), (i->pos+k*i->speed).toPointF());
		p.setMatrixEnabled(false);
		p.drawText(p.transform().map(i->pos.toPointF()), QString::number(i->sn));
		p.setMatrixEnabled(true);
#endif
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

