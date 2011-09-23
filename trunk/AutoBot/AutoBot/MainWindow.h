#ifndef AutoBot_H
#define AutoBot_H

#include <QtGui/QMainWindow>
#include "ui_AutoBot.h"

class AutoBotMainWindow : public QMainWindow
{
	Q_OBJECT

public:
	AutoBotMainWindow(QWidget *parent = 0, Qt::WFlags flags = 0);
	~AutoBotMainWindow();

private:
	Ui::AutoBotClass ui;
};

#endif // AutoBot_H
