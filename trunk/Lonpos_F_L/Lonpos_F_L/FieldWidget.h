#ifndef FIELDWIDGET_H
#define FIELDWIDGET_H

#include <QWidget>

#include "Figure.h"

/**	Отображает доступные клетки, установленниые фигуры, а так же текущую выбранную фигуру с привязкой к курсору.
  *	Не осуществляет никаких проверок.		*/
class FieldWidget : public QWidget
{
	Q_OBJECT

public:
	enum LayoutMode
	{
		MODE_VERTICAL,	///< Поле занимает всю ширину виджета, может появиться вертикальная полоса прокрутки
		MODE_FIT		///< Поля целиком помещается в виджет, полосы прокрутки не могут появиться
	};
	FieldWidget(QWidget *parent);
	~FieldWidget();

	void setWidth(int width);
	int width() const;
	void setHeight(int height);
	int height() const;

	void setLayoutMode(LayoutMode mode);
	void setFigures(Figure::list* figures);	///< Задать список отображаемых фигур
	void setFieldConfiguration(FieldPlace::list* fieldConfiguration); ///< Задать конфигурацию поля для отображения пустых ячеек на фоне

	/**	Задать дополнительную фигуру, которая будет отображаться поверх других.
	  *	Для временного отображения фигуры во время подбора места для неё. */
	void setOverlayFigure(const Figure& overlayFigure);
	void removeOverlayFigure();

	/**	Задать фигуру, изображение которой будет следовать за курсором. */
	void setTrackingFigure(const Figure&);
	void removeTrackingFigure();

signals:
	void lmbClicked(FieldPlace place);	///< Пользователь щёлкнул левой кнопкой по элементу поля place
	void rmbClicked();	///< Пользователь щёлкнул правой кнопкой
	void rotated(bool clockwise);	///< Пользователь инициировал поворот
	void removed();	///< Пользователь инициировал удаление фигуры с поля


private:
	LayoutMode m_layoutMode;
	Figure::list* m_figures;
	Figure m_overlayFigure;
	Figure m_trackingFigure;
};

#endif // FIELDWIDGET_H
