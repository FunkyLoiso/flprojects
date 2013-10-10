#include "lonpos_f_l.h"

#include <QString>

Lonpos_F_L::Lonpos_F_L(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);
	
	ui.fieldWidget->setLayoutMode(FieldWidget::MODE_VERTICAL);

	ui.fieldWidget->setLayoutMode(FieldWidget::MODE_FIT);
	ui.fieldWidget->setFieldWidth(10);
	ui.fieldWidget->setFieldHeight(10);

	m_fieldConf.append(FieldPlace(0, 0));
	m_fieldConf.append(FieldPlace(1, 1));
	m_fieldConf.append(FieldPlace(1, 3));
	m_fieldConf.append(FieldPlace(2, 1));
	m_fieldConf.append(FieldPlace(5, 5));
	m_fieldConf.append(FieldPlace(6, 5));

	ui.fieldWidget->setFieldConfiguration(&m_fieldConf);

	QString f =	"_O\n"
				"OXO\n"
				"_O";
	m_fieldFigures.append(Figure(f, FieldPlace(2, 2), Qt::red));
	f =	"__O\n"
		"_XO\n"	
		"_O\n"
		"OO";
	m_fieldFigures.append(Figure(f, FieldPlace(3, 3), Qt::blue));

	ui.fieldWidget->setFigures(&m_fieldFigures);
}

Lonpos_F_L::~Lonpos_F_L()
{

}
