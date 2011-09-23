#include "FieldViewWidget.h"
#include <QMouseEvent>

FieldViewWidget::FieldViewWidget( QWidget *parent /*= 0*/, int width /*= 8*/, int height /*= 8*/ ) :
m_width(width), m_height(height), QWidget(parent),
backgrowndColor("wheat"), usedColor("blue"), lineColor("blue"), blackFieldColor("sienna")
{
	m_knightImage.load("./Knight.png");
}


QString FieldViewWidget::GetFieldName( QPoint field )
{
	QString str;
	if(m_width <= 'Z' - 'A' + 1)
	{
		str += QChar(field.x() + 64);
	}
	else str += "гор: " + QString::number(field.x()) + " верт: ";

	str += QString::number(m_height - field.y() + 1);

	return str;
}


void FieldViewWidget::SetFieldSize( int height, int width )
{
	fieldDimsMutex.lock();
	m_width = width;
	m_height = height;
	fieldDimsMutex.unlock();
	update();
}

void FieldViewWidget::ShowField( const FieldState &field )
{
	fieldMutex.lock();
	this->m_field = field;
	fieldMutex.unlock();
	update();
}

QPointF FieldViewWidget::fieldPoint( unsigned int locX, unsigned int locY )
{
	return QPointF(m_fieldSize * locX, m_fieldSize * locY);
}

void FieldViewWidget::paintEvent( QPaintEvent * )
{
	QMutexLocker locker(&fieldDimsMutex);
	QPainter painter(this);

	float lineDeltaX = width() / (float)(m_width+2);
	float lineDeltaY = height() / (float)(m_height+2);

	m_fieldSize = qMin(lineDeltaX, lineDeltaY);

	/* Нарисуем поле */
	painter.setBrush(backgrowndColor);
	painter.drawRect(QRectF(0, 0, m_fieldSize*(m_width+2)-1, m_fieldSize*(m_height+2)-1));

	/* Нарисуем клетки */
	painter.setPen(Qt::SolidLine);
	QRectF checksFrame;
	checksFrame.setBottomLeft(fieldPoint(1, 1));
	checksFrame.setTopRight(fieldPoint(m_width+1, m_height+1));
	painter.drawRect(checksFrame);//рамка вокруг всех клеток

	painter.setBrush(blackFieldColor);
	//for(int y = 1; y <= m_height; ++y)
	//	for(int x = 1+y%2; x <= m_width; x+=2)
	//	{//нарисуем тёмную клетку
	//		QRectF rect;
	//		rect.setBottomLeft(fieldPoint(x, y));
	//		rect.setTopRight(fieldPoint(x+1, y+1));
	//		painter.drawRect(rect);
	//	}
	for(int y = 1; y <= m_height; ++y)
		for(int x = 1+y%2; x <= m_width; x+=2)
		{//нарисуем тёмную клетку
			QRectF rect;
			rect.setBottomLeft(fieldPoint(x, m_height-y+1));
			rect.setTopRight(fieldPoint(x+1, m_height-y+2));
			painter.drawRect(rect);
		}
		
	/* Нарисуем индексы */
	{//горизонтальные (буквы)
		QPointF shift(0.4*m_fieldSize, 1.6*m_fieldSize);
		if(m_width <= 'Z' - 'A' + 1)
		{
			for(int x = 1; x <= m_width; ++x)
			{
				painter.drawText(fieldPoint(x, m_height)+shift, QChar(x+64));
			}
		}
		else
		{
			for(int x = 1; x <= m_width; ++x)
			{
				painter.drawText(fieldPoint(x, m_height)+shift, QString::number(x));
			}
		}
	}
	
	{//вертикальные (цифры)
		QPointF shift(0.4*m_fieldSize, 0.6*m_fieldSize);
		for(int y = 1; y <= m_height; ++y)
		{
			painter.drawText(fieldPoint(0, y)+shift, QString::number(m_height - y + 1));
		}
	}

	/* Нарисуем путь */
	painter.setRenderHints(QPainter::Antialiasing, true);
	
	{	
		float markerRadius = m_fieldSize/20;
		fieldMutex.lock();
		FieldState tmp = m_field;
		fieldMutex.unlock();

		//QPen pen;
		//pen.setColor(lineColor);
		//pen.setWidthF(markerRadius);
		//painter.setPen(pen);

		QColor brCol = usedColor;
		brCol.setAlpha(100);
		painter.setBrush(brCol);

		QPen fieldPen;
		fieldPen.setColor(brCol);
		fieldPen.setWidthF(0);

		QPen linePen;
		linePen.setColor(lineColor);
		linePen.setWidthF(markerRadius);
		if(tmp.size() > 1)
		{
			for(FieldState::Iterator i = tmp.begin()+1; i != tmp.end(); ++i)
			{//для каждого поля из пути
				QPoint &cur = *i;
				QPoint &prev = *(i-1);

				//подкрасим поле
				painter.setPen(fieldPen);
				QRectF rect;
				rect.setBottomLeft(fieldPoint(prev.x(), prev.y()));
				rect.setTopRight(fieldPoint(prev.x()+1, prev.y()+1));
				painter.drawRect(rect);

				//нарисуем линию и кружок
				painter.setPen(linePen);
				painter.drawLine(fieldPoint(prev.x(), prev.y()) + QPoint(0.5*m_fieldSize, 0.5*m_fieldSize), fieldPoint(cur.x(), cur.y()) + QPoint(0.5*m_fieldSize, 0.5*m_fieldSize));
				painter.drawEllipse(fieldPoint(prev.x(), prev.y()) + QPoint(0.5*m_fieldSize, 0.5*m_fieldSize), markerRadius, markerRadius);
			}
		}

		//нарисуем коня в последнем квадратике
		if(tmp.size() > 0)
		{
			QPoint &topLeft = *(tmp.end()-1);
			QRectF pixmapRect;
			pixmapRect.setTopLeft(fieldPoint(topLeft.x(), topLeft.y()));
			pixmapRect.setBottomRight(fieldPoint(topLeft.x()+1, topLeft.y()+1));
			painter.drawPixmap(pixmapRect.toAlignedRect(), m_knightImage);
			painter.drawRect(pixmapRect);
		}

	}
	
}

void FieldViewWidget::mouseReleaseEvent( QMouseEvent *mouseEvent)
{
	int x = mouseEvent->x();
	int y = mouseEvent->y();
	
	int lineX = x/m_fieldSize;
	int lineY = y/m_fieldSize;

	if(lineX < 1 || lineX > m_width || lineY < 1 || lineY > m_height) return;

	mouseEvent->accept();
	emit fieldClicked(QPoint(lineX, lineY));
}
