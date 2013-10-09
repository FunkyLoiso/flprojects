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
	Figure(const QString& configuration, QColor color);

	bool isValid() const;	///< �������� �� ������ ����������.
	void setConfiguration(const QString& configuration);
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
	enum{R0, R90, R180, R270} m_rotation;	//������� ������
};
#endif // Figure_h__
