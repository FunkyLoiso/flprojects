#include "romancalc.h"

#include "RomanNumber.h"

RomanCalc::RomanCalc(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags), m_op(0)
{
	ui.setupUi(this);

	m_bGroupDigs = new QButtonGroup(this);
	m_bGroupDigs->addButton(ui.bI);
	m_bGroupDigs->addButton(ui.bV);
	m_bGroupDigs->addButton(ui.bX);
	m_bGroupDigs->addButton(ui.bL);
	m_bGroupDigs->addButton(ui.bC);
	m_bGroupDigs->addButton(ui.bD);
	m_bGroupDigs->addButton(ui.bM);

	connect(m_bGroupDigs, SIGNAL(buttonClicked(QAbstractButton *)), this, SLOT(digitPresed(QAbstractButton *)));

	m_bGroupOps = new QButtonGroup(this);
	m_bGroupOps->addButton(ui.bEquals);
	m_bGroupOps->addButton(ui.bPlus);
	m_bGroupOps->addButton(ui.bMinus);
	m_bGroupOps->addButton(ui.bMul);

	connect(m_bGroupOps, SIGNAL(buttonClicked(QAbstractButton *)), this, SLOT(opPressed(QAbstractButton *)));

	connect(ui.bBackspace, SIGNAL(clicked()), this, SLOT(backspace()));
	connect(ui.bAllClear, SIGNAL(clicked()), this, SLOT(ac()));
}

RomanCalc::~RomanCalc()
{

}

void RomanCalc::digitPresed(QAbstractButton *b)
{
	RomanNumber& entering = (m_op == 0 ? m_left : m_right);
	QString cur = entering.isValid() ? entering.toString() : QString();
	cur.append(b->text());
	RomanNumber test(cur);
	if(test.isValid())
	{
		entering = test;
		ui.lineCurrent->setText(cur);
	}
	else
	{
		qApp->beep();
	}
}

void RomanCalc::opPressed(QAbstractButton *b)
{
	if(m_op == 0)//ввели левое число
	{
		if(!m_left.isValid() || b->text()[0] == '=')
		{
			qApp->beep();
			return;
		}

		m_op = b->text()[0];
		ui.listHistory->insertItem(0, m_left.toString() + " " + m_op + " ");
	}

	else//ввели правое число
	{
		if(!m_right.isValid())
		{
			qApp->beep();
			return;
		}

		RomanNumber result;
		if(m_op == '+') result = m_left + m_right;
		else if(m_op == '-') result = m_left - m_right;
		else if(m_op == '*') result = m_left * m_right;
		else
		{
			qApp->beep();
			return;	
		}

		if(!result.isValid())
		{
			qApp->beep();
			return;	
		}

		m_left = result;
		ui.lineCurrent->setText(m_left.toString());

		QString oldText = ui.listHistory->item(0)->text();
		ui.listHistory->item(0)->setText(oldText + m_right.toString() + " = " + result.toString());

		m_right = RomanNumber();
		if(b->text() != "=")
		{
			m_op = b->text()[0];
			ui.listHistory->insertItem(0, m_left.toString() + " " + m_op + " ");
		}
		else
		{
			m_op = 0;
		}
	}
}

void RomanCalc::backspace()
{
	RomanNumber& entering = (m_op == 0 ? m_left : m_right);
	if(entering.isValid())
	{
		QString newStr = entering.toString();
		newStr.chop(1);
		entering = RomanNumber(newStr);
		ui.lineCurrent->setText(entering.isValid() ? entering.toString() : QString());
	}
}

void RomanCalc::ac()
{
	m_left = RomanNumber();
	m_right = RomanNumber();
	m_op = 0;
	ui.lineCurrent->setText(QString());
}
