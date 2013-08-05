#ifndef STORMY_H
#define STORMY_H

#include <QtGui/QMainWindow>
#include "ui_stormy.h"

class Stormy : public QMainWindow
{
	Q_OBJECT

public:
	Stormy(QWidget *parent = 0, Qt::WFlags flags = 0);
	~Stormy();

private:
	Ui::StormyClass ui;
};

#endif // STORMY_H
