#ifndef LEDCUBEDATASOURCE_H
#define LEDCUBEDATASOURCE_H

#include <QtGui/QMainWindow>
#include "ui_ledcubedatasource.h"

#include "serial.h"

class LedCubeDataSource : public QMainWindow
{
	Q_OBJECT

public:
	LedCubeDataSource(QWidget *parent = 0, Qt::WFlags flags = 0);
	~LedCubeDataSource();

private slots:
	void onSendMagic();
	void onSendFrame();
	void onSendOther();

	void onReceive();


private:
	Ui::LedCubeDataSourceClass ui;

	Serial m_serial;

	void appendSent(QString str);
};

#endif // LEDCUBEDATASOURCE_H
