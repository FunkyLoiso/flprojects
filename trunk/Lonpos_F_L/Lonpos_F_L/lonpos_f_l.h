#ifndef LONPOS_F_L_H
#define LONPOS_F_L_H

#include <QtGui/QMainWindow>
#include "ui_lonpos_f_l.h"

#include "Figure.h"
#include "FieldController.h"

class Lonpos_F_L : public QMainWindow
{
	Q_OBJECT

public:
	Lonpos_F_L(QWidget *parent = 0, Qt::WFlags flags = 0);
	~Lonpos_F_L();

private:
	Ui::Lonpos_F_LClass ui;

	Figure::list m_fieldFigures;
	Figure::list m_spareFigures;
	FieldPlace::list m_fieldConf;

	FieldController m_controller;
};

#endif // LONPOS_F_L_H
