#ifndef Figure_h__
#define Figure_h__

#include <QPoint>
#include <QString>
#include <QList>
#include <QColor>
#include <QHash>
#include <QUuid>

/**	������� �� ���� � ����������� ����. ������� ����� �� ������ � ������������ � �������� */
class FieldPlace : public QPoint
{
public:
	typedef QList<FieldPlace> list;

	FieldPlace() : QPoint(INT_MIN, INT_MIN) {}
	FieldPlace(int x, int y) : QPoint(x, y) {}
	FieldPlace(const QPoint& pt) : QPoint(pt) {}
	
	static FieldPlace Invalid() {return FieldPlace();}
	bool isValid() const {return *this != Invalid();}
};

static uint qHash(const FieldPlace& place)
{
	return qHash(place.x()) ^ qHash(place.y());
}


/**	������. �������� ����, �������������� ������ � ���� ��������� ���������, ����� ���� ��������. */
class Figure
{
public:
	typedef QList<Figure> list;

	Figure();	///< ������������ ������
	static Figure Invalid() {return Figure();}; ///< ���������� ������������ ������

	/**	������, �������� �������. ������ ����� ������� �������� '_', ������� �������� 'O', ����������� ������� �������� 'X'.
	  *	������� ������ ����������� �������� '\n'. ������:
	  *	QString figure =	"_O\n"
							"OXO\n"
							"_O";			*/
	Figure(const QString& configuration, QColor color, FieldPlace center = FieldPlace::Invalid());
	bool operator==(const Figure& figure) const;
	bool operator!=(const Figure& figure) const;

	bool isValid() const;	///< �������� �� ������ ����������.
	void setColor(QColor color);
	QColor color() const;
	int width() const;	///< ������������ ����� ��������� �� �����������
	int height() const;	///< ������������ ����� ��������� �� ���������
	FieldPlace::list elements() const; ///< ������ ��������� ���� ��������� ������, ������� �����
	FieldPlace center() const;
	void setCenter(FieldPlace place);
	void move(int dx, int dy); ///< ����������� ������ �� dx �� ����������� � �� dy �� ���������
	void setUpperLeft(FieldPlace place);	///< �������� ����� ���, ����� ������� ����� ���� ������ �������� � place

	void rotateCW();	///< ��������� ������ �� 90 �������� �� ������� �������
	void rotateCCW();	///< ��������� ������ �� 90 �������� ������ ������� �������	

private:
	QUuid m_id;
	QColor m_color;
	FieldPlace m_center;	///< ���������� ������
	FieldPlace::list m_relativeElements;	//�������� ��������� ��������� ������������ ������
	int m_width, m_height;

	bool setConfiguration(const QString& configuration);
	void rotate(bool clockwise);
	void updateSize();	///< ����������� ��� ������ ������ � ������
};
#endif // Figure_h__
