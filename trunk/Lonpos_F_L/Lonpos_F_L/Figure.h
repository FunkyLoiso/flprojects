#ifndef Figure_h__
#define Figure_h__

#include <QPoint>
#include <QString>
#include <QList>
#include <QColor>

/**	Позиция на поле в координатах поля. Введена чтобы не путать с координатами в пикселах */
class FieldPlace : public QPoint
{
public:
	FieldPlace() : QPoint() {}
	FieldPlace(int x, int y) : QPoint(x, y) {}
	FieldPlace(const QPoint& pt) : QPoint(pt) {}
	typedef QList<FieldPlace> list;
};


/**	Фигура. Содержит цвет, местоположение центра и всех остальных элементов, может быть повёрнута. */
class Figure
{
public:
	typedef QList<Figure> list;

	Figure();	///< Недопустимая фигура
	static Figure InvalidFigure() {return Figure();}; ///< Возвращает недопустимую фигуру

	/**	Фигура, заданная строкой. Пустое место задаётся символом '_', элемент символом 'O', центральный элемент символом 'X'.
	  *	Строчки фигуры разделяются символом '\n'. Пример:
	  *	QString figure =	"_O\n"
							"OXO\n"
							"_O";			*/
	Figure(const QString& configuration, FieldPlace center, QColor color);

	bool isValid() const;	///< Является ли фигура допустимой.
	bool setConfiguration(const QString& configuration);
	void setColor(QColor color);
	QColor color() const;
	int width() const;	///< Максимальное число элементов по горизонтали
	int height() const;	///< Максимальное число элементов по вертикали
	FieldPlace::list places() const; ///< Список координат всех элементов фигуры, включая центр
	FieldPlace center() const;
	void setCenter(FieldPlace place);
	void move(int dx, int dy); ///< Передвинуть фигуру на dx по горизонтали и на dy по вертикали

	void rotateCW();	///< Повернуть фигуру на 90 градусов по часовой стрелке
	void rotateCCW();	///< Повернуть фигуру на 90 градусов против часовой стрелке	

private:
	QColor m_color;
	FieldPlace m_center;	///< Координаты центра
	FieldPlace::list m_relativeElements;	//Смещения остальных элементов относительно центра
	int m_width, m_height;

	void rotate(bool clockwise);
};
#endif // Figure_h__
