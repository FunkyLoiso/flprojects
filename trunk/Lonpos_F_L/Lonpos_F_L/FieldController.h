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

	void addSpareFigure(const Figure& spareFigure);	///< Добавить фигуру в список возможных к установке на поле
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

private slots:
	void onSpareFiguresLmbClicked(FieldPlace place);
	void onSpareFiguresRmbClicked();
	void onFieldLmbClicked(FieldPlace place);
	void onFieldRmbClicked();
	void onFieldRotated();	///< команда повернуть выбранную фигуру
	void onFieldRemoved();	///< команда удалить выбранную фигуру
};
#endif // FieldController_h__
