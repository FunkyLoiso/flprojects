#ifndef LEDQBEMU_H
#define LEDQBEMU_H

#include <QtGui/QMainWindow>
#include "ui_ledqbemu.h"

class QBWidget;

class LedQbEmu : public QMainWindow
{
	Q_OBJECT

public:
	LedQbEmu(QWidget *parent = 0, Qt::WFlags flags = 0);
	~LedQbEmu();

private:
	void timerEvent(QTimerEvent *);

	Ui::LedQbEmuClass ui;

	QBWidget* m_qbWidget;
};

#endif // LEDQBEMU_H
