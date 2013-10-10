#ifndef Figure_h__
#define Figure_h__

#include <QPoint>
#include <QString>
#include <QList>
#include <QColor>

/**	������� �� ���� � ����������� ����. ������� ����� �� ������ � ������������ � �������� */
class FieldPlace : public QPoint
{
public:
	FieldPlace() : QPoint() {}
	FieldPlace(int x, int y) : QPoint(x, y) {}
	FieldPlace(const QPoint& pt) : QPoint(pt) {}
	typedef QList<FieldPlace> list;
};


/**	������. �������� ����, �������������� ������ � ���� ��������� ���������, ����� ���� ��������. */
class Figure
{
public:
	typedef QList<Figure> list;

	Figure();	///< ������������ ������
	static Figure InvalidFigure() {return Figure();}; ///< ���������� ������������ ������

	/**	������, �������� �������. ������ ����� ������� �������� '_', ������� �������� 'O', ����������� ������� �������� 'X'.
	  *	������� ������ ����������� �������� '\n'. ������:
	  *	QString figure =	"_O\n"
							"OXO\n"
							"_O";			*/
	Figure(const QString& configuration, FieldPlace center, QColor color);

	bool isValid() const;	///< �������� �� ������ ����������.
	bool setConfiguration(const QString& configuration);
	void setColor(QColor color);
	QColor color() const;
	int width() const;	///< ������������ ����� ��������� �� �����������
	int height() const;	///< ������������ ����� ��������� �� ���������
	FieldPlace::list places() const; ///< ������ ��������� ���� ��������� ������, ������� �����
	FieldPlace center() const;
	void setCenter(FieldPlace place);
	void move(int dx, int dy); ///< ����������� ������ �� dx �� ����������� � �� dy �� ���������

	void rotateCW();	///< ��������� ������ �� 90 �������� �� ������� �������
	void rotateCCW();	///< ��������� ������ �� 90 �������� ������ ������� �������	

private:
	QColor m_color;
	FieldPlace m_center;	///< ���������� ������
	FieldPlace::list m_relativeElements;	//�������� ��������� ��������� ������������ ������
	int m_width, m_height;

	void rotate(bool clockwise);
};
#endif // Figure_h__
