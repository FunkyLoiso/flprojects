#ifndef TRIPODTEST_H
#define TRIPODTEST_H

#include <QtGui/QMainWindow>
#include "ui_tripodtest.h"

#include "ReaderThread.h"
#include "serial.h"

class TripodTest : public QMainWindow
{
	Q_OBJECT

public:
	TripodTest(QWidget *parent = 0, Qt::WFlags flags = 0);
	~TripodTest();

private slots:
	void portMenuAboutToOpen();///< Наполнить список com-портов
	void onPortSelected(QAction *);///< Обработать смену порта
	void onValuesChanged(short, short, short);
	void onZeroCalibrate();

private:
	void onPortChaned();///< Установлен новый порт. Do the job

	Ui::TripodTestClass ui;

	ReaderThread m_readerThread;
	Serial m_port;
	short m_v0, m_v3, m_v5;
	short m_c0, m_c3, m_c5;
};

#endif // TRIPODTEST_H
