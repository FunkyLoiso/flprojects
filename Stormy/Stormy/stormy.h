#ifndef STORMY_H
#define STORMY_H

#include <QMainWindow>
#include <QElapsedTimer>
#include "ui_stormy.h"

#include "Glass.h"
#include "SimplePhysicsEngine.h"
#include "SimplePhysicsThread.h"

class QLabel;

class Stormy : public QMainWindow
{
	Q_OBJECT

public:
	Stormy(QWidget *parent = 0, Qt::WindowFlags flags = 0);
	~Stormy();

private:
	Ui::StormyClass ui;

	Glass m_glass;
	SimplePhysicsEngine m_engine;
	SimplePhysicsThread m_thread;

	QElapsedTimer m_timer;
	quint64 m_lastTime;

	QLabel* m_fpsLabel;

	//QVector<Particle&> m_addQueue;

private slots:
	void glassWasUpdated();
	void onButton1();
};

#endif // STORMY_H
