#include "Figure.h"
#include <QSet>

Figure::Figure(void)
: m_center(0, 0)
{
}

Figure::Figure(const QString& configuration, QColor color, FieldPlace center/* = FieldPlace::Invalid()*/)
: m_color(color)
{
	if(setConfiguration(configuration))
	{
		m_id = QUuid::createUuid();
		if(center.isValid()) setCenter(center);
	}
}

bool Figure::isValid() const
{
	return m_id != QUuid();
}

bool Figure::setConfiguration(const QString& configuration)
{
	if(configuration.count('X') != 1) return false;

	//найдём координаты центра (X)
	int centerX = 0, centerY = 0;
	for(int c = 0; c < configuration.length(); ++c)
	{
		const QChar& ch = configuration.at(c);
		if(ch == '\n')
		{
			++centerY;
			centerX = 0;
			continue;
		}
		else if(ch == 'X')
		{
			break;
		}
		++centerX;
	}
	m_center = FieldPlace(centerX, centerY);

	//разделим конфигурацию на строчки и найдём относительные координаты всех элементов (O)
	//за одно определим минимальные и максимальные координаты, чтобы потом высчтитать ширину и высоту
	QStringList lines = configuration.split('\n');
	m_height = lines.count();
	for(int y = 0; y < lines.count(); ++y)
	{
		const QString& line = lines.at(y);
		for(int x = 0; x < line.length(); ++x)
		{
			if(line.at(x) == 'O')
			{
				FieldPlace relativePlace(FieldPlace(x, y) - m_center);
				m_relativeElements.append(relativePlace);
			}
		}
	}
	updateSize();
	return true;
}

void Figure::setColor(QColor color)
{
	m_color = color;
}

QColor Figure::color() const
{
	return m_color;
}

int Figure::width() const
{
	return m_width;
}

int Figure::height() const
{
	return m_height;
}

FieldPlace::list Figure::elements() const
{
	FieldPlace::list result;
	result.append(m_center);
	Q_FOREACH(const FieldPlace& place, m_relativeElements)
	{
		result.append(m_center + place);
	}
	return result;
}

FieldPlace Figure::center() const
{
	return m_center;
}

void Figure::setCenter(FieldPlace place)
{
	m_center = place;
}

void Figure::move(int dx, int dy)
{
	m_center += FieldPlace(dx, dy);
}

void Figure::setUpperLeft(FieldPlace place)
{
	int minX(INT_MAX), minY(INT_MAX);
	Q_FOREACH(FieldPlace place, elements())
	{
		if(minX > place.x()) minX = place.x();
		if(minY > place.y()) minY = place.y();
	}

	move(place.x()-minX, place.y()-minY);
}


void Figure::rotateCW()
{
	rotate(true);
}

void Figure::rotateCCW()
{
	rotate(false);
}

void Figure::rotate(bool clockwise)
{
	for(FieldPlace::list::Iterator i = m_relativeElements.begin(); i != m_relativeElements.end(); ++i)
	{
		int temp = i->x();
		if(clockwise)	//  (1; -3) -> (3; 1)
		{
			i->rx() = -i->y();
			i->ry() = temp;
		}
		else			//	(1; -3) -> (-3, -1)
		{
			i->rx() = i->y();
			i->ry() = -temp;
		}
	}
	updateSize();//размеры могли изменться после поворота
}

bool Figure::operator==(const Figure& figure) const
{
	return	m_id == figure.m_id;
}

bool Figure::operator!=(const Figure& figure) const
{
	return !operator==(figure);
}

void Figure::updateSize()
{
	int minX(INT_MAX), minY(INT_MAX), maxX(INT_MIN), maxY(INT_MIN);
	Q_FOREACH(FieldPlace place, elements())
	{
		if(minX > place.x()) minX = place.x();
		if(maxX < place.x()) maxX = place.x();
		if(minY > place.y()) minY = place.y();
		if(maxY < place.y()) maxY = place.y();
	}

	m_width = maxX - minX + 1;
	m_height = maxY - minY + 1;
}
