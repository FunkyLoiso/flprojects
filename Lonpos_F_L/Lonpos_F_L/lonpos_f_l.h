#ifndef LONPOS_F_L_H
#define LONPOS_F_L_H

#include <QtGui/QMainWindow>
#include "ui_lonpos_f_l.h"

class Lonpos_F_L : public QMainWindow
{
	Q_OBJECT

public:
	Lonpos_F_L(QWidget *parent = 0, Qt::WFlags flags = 0);
	~Lonpos_F_L();

private:
	Ui::Lonpos_F_LClass ui;
};

#endif // LONPOS_F_L_H
