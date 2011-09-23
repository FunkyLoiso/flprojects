#ifndef KNIGHTSTOUR_H
#define KNIGHTSTOUR_H

#include <QtGui/QMainWindow>
#include "ui_knightstour.h"

#include "FieldViewWidget.h"
#include "KnightsTourSolver.h"

#include <QTimer>

class KnightsTour : public QMainWindow
{
	Q_OBJECT

public:
	KnightsTour(QWidget *parent = 0, Qt::WFlags flags = 0);
	~KnightsTour();

private:
	Ui::KnightsTourClass ui;
	//FieldViewWidget m_field;	///< Виджет с полем
	FieldState m_moves;	///< Последовательность ходов
	int m_width, m_height;

	KnightsTourSolver m_solver;

	QTimer m_timer;

private slots:
	void onSetFieldSize();
	void onFieldClick(QPoint field);
	void onForward();

	void addMoveToList( QPoint next );

	void onBack();
	void onDelayChanged(int newDelay);
	void onGo();
	void onStop();

	void onExit();
	void onAbout();
	void onAboutQt();
};

#endif // KNIGHTSTOUR_H
