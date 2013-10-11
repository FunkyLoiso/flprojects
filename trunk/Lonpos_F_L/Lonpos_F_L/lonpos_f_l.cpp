#include "lonpos_f_l.h"

#include <QString>

Lonpos_F_L::Lonpos_F_L(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);
	
	ui.spareFiguresWidget->setLayoutMode(FieldWidget::MODE_VERTICAL);
	ui.fieldWidget->setLayoutMode(FieldWidget::MODE_FIT);

	//m_fieldConf.append(FieldPlace(0, 0));
	//m_fieldConf.append(FieldPlace(1, 1));
	//m_fieldConf.append(FieldPlace(1, 3));
	//m_fieldConf.append(FieldPlace(2, 1));
	//m_fieldConf.append(FieldPlace(5, 5));
	//m_fieldConf.append(FieldPlace(6, 5));

	QString field =	"__O\n"
					"_OOO\n"
					"OOOOO\n"
					"_OOO\n"
					"__O";
	m_controller.setFieldConfiguration(field);


	QString f =	"_O\n"
				"OXO\n"
				"_O";

	m_controller.addSpareFigure(Figure(f, Qt::red));
	f =	"__O\n"
		"_XO\n"	
		"_O\n"
		"OO";
	m_controller.addSpareFigure(Figure(f, Qt::blue));
	f =	"XO\n"
		"O\n";
	m_controller.addSpareFigure(Figure(f, Qt::green));

	m_controller.connectFieldWidget(ui.fieldWidget);
	m_controller.connectSpareFiguresWidget(ui.spareFiguresWidget);
}

Lonpos_F_L::~Lonpos_F_L()
{

}
