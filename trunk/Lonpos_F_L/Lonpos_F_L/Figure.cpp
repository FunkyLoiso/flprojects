#include "Figure.h"

Figure::Figure(void)
{
}

Figure::Figure(const QString& configuration, QColor color)
: m_color(color)
{
	setConfiguration(configuration);
}

bool Figure::isValid() const
{
	return m_relativeElements.count() > 0;
}

bool Figure::setConfiguration(const QString& configuration)
{
	if(configuration.count('X') != 1) return false;

	//����� ���������� ������ (X)
	int centerX = 0, centerY = 0;
	for(int c = 0; c < configuration.length(); ++c)
	{
		const QChar& ch = configuration.at(c);
		if(ch == '\n')
		{
			++centerY;
			centerX = 0;
		}
		else if(ch == 'X')
		{
			break;
		}
		++centerX;
	}
	m_center = FieldPlace(centerX, centerY);

	int minX, minY, maxX, maxY;
	minX = maxX = centerX;
	minY = maxY = centerY;
	//�������� ������������ �� ������� � ����� ������������� ���������� ���� ��������� (O)
	//�� ���� ��������� ����������� � ������������ ����������, ����� ����� ���������� ������ � ������
	QStringList lines = configuration.split('\n');
	m_height = lines.count();
	for(int y = 0; y < lines.count(); ++y)
	{
		const QString& line = lines.at(y);
		for(int x = 0; x < line.length(); ++x)
		{
			if(line.at(x) == 'O')
			{
				FieldPlace relativePlace(m_center - FieldPlace(x, y));
				m_relativeElements.append(relativePlace);

				if(minX > x) minX = x;
				if(maxX < x) maxX = x;
				if(minY > y) minY = y;
				if(maxY < y) maxY = y;
			}
		}
	}
	m_width = maxX - minX + 1;
	m_height = maxY = minY + 1;

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

FieldPlace::list Figure::places() const
{
	FieldPlace::list result;
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
}
