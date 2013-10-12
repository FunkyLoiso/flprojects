#include "lonpos_f_l.h"

#include <QString>
#include <QKeyEvent>

Lonpos_F_L::Lonpos_F_L(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);
	
	ui.spareFiguresWidget->setLayoutMode(FieldWidget::MODE_VERTICAL);
	ui.fieldWidget->setLayoutMode(FieldWidget::MODE_FIT);

	//поле
	QString field =	"__O\n"
					"_OOO\n"
					"OO_OO\n"
					"_OOO\n"
					"__O";
	m_controller.setFieldConfiguration(field);

	//фигуры
	QString f =	"_O\n"
				"OX\n"
				"_O";
	m_controller.addSpareFigure(Figure(f, Qt::red));

	f =	"__O\n"
		"_XO\n"	
		"OO\n";
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

void Lonpos_F_L::keyPressEvent(QKeyEvent *e)
{
	if(e->key() == Qt::Key_Delete || e->key() == Qt::Key_Backspace) m_controller.onFieldRemoved();
	else if(e->key() == Qt::Key_Space) emit m_controller.onFieldRotated(true);
	else if(e->key() == Qt::Key_Shift) emit m_controller.onFieldRotated(false);
	else if(e->key() == Qt::Key_Escape) emit m_controller.onCancelSelection();
	else __super::keyPressEvent(e);
}

void Lonpos_F_L::wheelEvent(QWheelEvent *e)
{
	int numDegrees = e->delta() / 8;
	int numSteps = numDegrees / 15;

	for(int step = 0; step < qAbs(numSteps); ++step)
	{
		emit m_controller.onFieldRotated(numSteps > 0);
	}
	e->accept();
}
