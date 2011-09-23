#ifndef FIELDVIEWWIDGET_H
#define FIELDVIEWWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QMutex>
#include <QQueue>

#include "./common.h"

/**	Виджет для отображения поля и последовательонсти ходов. */
class  FieldViewWidget : public QWidget
{
    Q_OBJECT
public:
    FieldViewWidget(QWidget *parent = 0, int width = 8, int height = 8);

	QString GetFieldName(QPoint field);	///< Получить имя поля с координатами field

public slots:
	void SetFieldSize(int height, int width);	///< Изменить число клеток на поле
	void ShowField(const FieldState &field);	///< Передать новое поле и отобразить его

signals:
	void fieldClicked(QPoint field);	///< Пользователь щёлкнул по полю

private:

    int m_width;	///< Число клеток по горизонтали
	int m_height;	///< Число клеток по вертикали
    QColor backgrowndColor, usedColor, lineColor, blackFieldColor; //цвета разных штук
	float m_fieldSize;	///< Ширина клетки в пикселах
    FieldState m_field;	///< Текущий порядок ходов на поле
    QMutex fieldMutex;	///< Мьютекс изменения состояния поля
	QMutex fieldDimsMutex;	///< Мьютекс изменения размеров поля

	QPixmap m_knightImage;	///< Картинка с конём

    QPointF fieldPoint(unsigned int locX, unsigned int locY);	///< Получить координаты пересечения границ полей
    void paintEvent(QPaintEvent *);	///< Перерисовка виджета
	void mouseReleaseEvent(QMouseEvent *);	///< Определяет, был ли это щелчёк по полю, и пускает сигнал fieldClicked, если так
};

#endif