#ifndef FieldController_h__
#define FieldController_h__

#include <QObject>
#include "Figure.h"
#include "FieldWidget.h"

/**	Контроллер поля.
  *	Содержит все фигуры, как находящиеся на поле, так и доступные для вставки. Проверяет допустимость
  *	вставки фигур в поле, управляет отображением фигуры в процессе подбора позиции. */
class FieldController : QObject
{
	Q_OBJECT
public:
	FieldController();

	void addSpareFigure(Figure spareFigure);	///< Добавить фигуру в список возможных к установке на поле
	void addFieldFigure(const Figure& fieldFigure);	///< Добавить фигуру в список установленных на поле

	/**	Задать конфигурацию поля ввиде сторки. Недопустимые ячейки обозначаются символом '_', допустимые - 'O'.
	  *	Строки отделятся символом '\n'. Пример:
	  *	QString field =	"__O\n"
						"_OOO\n"
						"OOOO\n"
						"OOO\n"
						"_O"			*/
	void setFieldConfiguration(const QString& configuration);

	void connectSpareFiguresWidget(FieldWidget* spareFiguresWidget);///< Соединить с виджетом доступных для добавления фигур
	void disconnectSpareFiguresWidget();
	void connectFieldWidget(FieldWidget* fieldWidget);	///< Соединить с виджетом поля
	void disconnectFieldWidget();

public slots:
	void onSpareFiguresLmbClicked(FieldPlace place);	///< Щелчок левой кнопкой по элементу place поля
	void onFieldLmbClicked(FieldPlace place);			///< Щелчок левой кнопкой по элементу place списка неиспользованных фигур
	void onCancelSelection();
	void onFieldRotated(bool clockwise);	///< команда повернуть выбранную фигуру
	void onFieldRemoved();	///< команда удалить выбранную фигуру

private:
	Figure::list m_fieldFigures;
	Figure::list m_spareFigures;
	int m_spareFiguresFieldWidth, m_spareFiguresFieldHeight;
	FieldPlace::list m_fieldConf;
	int m_fieldWidth, m_fieldHeight;

	FieldWidget* m_spareFiguresWidget;
	FieldWidget* m_fieldWidget;

	Figure m_fieldFigureSelected;
	Figure m_spareFigureSelected;

	void clearFieldSelection();
	void clearSpareSelection();

	bool canBePlaced(const Figure& figureToPlace, FieldPlace newPlace) const;
	Figure::list::Iterator getFigureInPlace(FieldPlace place);
	void updateSpareFiguresList();
};
#endif // FieldController_h__
