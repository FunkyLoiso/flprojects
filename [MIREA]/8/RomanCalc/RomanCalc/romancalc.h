#ifndef ROMANCALC_H
#define ROMANCALC_H

#include <QtGui/QMainWindow>
#include "ui_romancalc.h"
#include "RomanNumber.h"

class RomanCalc : public QMainWindow
{
	Q_OBJECT

public:
	RomanCalc(QWidget *parent = 0, Qt::WFlags flags = 0);
	~RomanCalc();

private slots:
	void digitPresed(QAbstractButton *b);
	void opPressed(QAbstractButton *b);
	void backspace();
	void ac();

private:
	Ui::RomanCalcClass ui;

	QButtonGroup *m_bGroupDigs;
	QButtonGroup *m_bGroupOps;

	RomanNumber m_left, m_right;
	QChar m_op;
};

#endif // ROMANCALC_H
