#include "knightstour.h"
#include <QMessageBox>

KnightsTour::KnightsTour(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);

	onSetFieldSize();

	connect(ui.buttonSetFieldSize, SIGNAL(clicked()), this, SLOT(onSetFieldSize()));
	connect(ui.fieldWidget, SIGNAL(fieldClicked(QPoint)), this, SLOT(onFieldClick(QPoint)));
	connect(ui.buttonForward, SIGNAL(clicked()), this, SLOT(onForward()));
	connect(ui.buttonBack, SIGNAL(clicked()), this, SLOT(onBack()));
	connect(ui.spinboxDelay, SIGNAL(valueChanged(int)), this, SLOT(onDelayChanged(int)));
	connect(ui.buttonGo, SIGNAL(clicked()), this, SLOT(onGo()));
	connect(ui.buttonStop, SIGNAL(clicked()), this, SLOT(onStop()));
	connect(ui.radioManual, SIGNAL(clicked()), this, SLOT(onStop()));

	connect(ui.buttonExit, SIGNAL(clicked()), this, SLOT(onExit()));
	connect(ui.actionAbout, SIGNAL(triggered()), this, SLOT(onAbout()));
	connect(ui.actionAbout_Qt, SIGNAL(triggered()), this, SLOT(onAboutQt()));

	m_timer.setSingleShot(false);

	connect(&m_timer, SIGNAL(timeout()), this, SLOT(onForward()));

	ui.fieldWidget->ShowField(m_moves);
}

KnightsTour::~KnightsTour()
{

}

void KnightsTour::onSetFieldSize()
{
	m_width = ui.spinboxWidth->value();
	m_height = ui.spinBoxHeight->value();

	ui.fieldWidget->SetFieldSize(m_height, m_width);
	m_solver.SetFieldSize(m_height, m_width);

	m_moves.clear();
	ui.fieldWidget->ShowField(m_moves);

	ui.listMoves->clear();
}

void KnightsTour::onFieldClick( QPoint field )
{
	m_moves.clear();
	ui.listMoves->clear();
	m_solver.SetFieldSize(m_height, m_width);
	m_solver.SetStartPos(field);

	m_moves.push_back(field);
	//ui.listMoves->addItem(QString("1: %1").arg(ui.fieldWidget->GetFieldName(field)));
	addMoveToList(field);

	ui.fieldWidget->ShowField(m_moves);
}

void KnightsTour::onForward()
{
	QPoint next = m_solver.GetNextMove();
	if(next.x() == -1)
	{
		m_timer.stop();
		//QMessageBox::information(this, "Конец", "Ходить больше некуда.. Получилось ли..?");
	}
	else
	{
		m_moves.push_back(next);
		ui.fieldWidget->ShowField(m_moves);

		addMoveToList(next);

	}
}

void KnightsTour::onBack()
{
	if(m_moves.size() > 1) m_moves.resize(m_moves.size() - 1);

	m_solver.ResetField(m_moves);
	ui.fieldWidget->ShowField(m_moves);

	delete ui.listMoves->takeItem(ui.listMoves->model()->rowCount()-1);
	ui.listMoves->setCurrentRow(ui.listMoves->model()->rowCount()-1);
	
}


void KnightsTour::onDelayChanged( int newDelay )
{
	m_timer.setInterval(ui.spinboxDelay->value());
}


void KnightsTour::onGo()
{
	m_timer.setInterval(ui.spinboxDelay->value());

	m_timer.start();
}

void KnightsTour::onStop()
{
	m_timer.stop();	
}

void KnightsTour::addMoveToList( QPoint move )
{
	ui.listMoves->addItem(QString("%1: %2").arg(ui.listMoves->model()->rowCount()+1).arg(ui.fieldWidget->GetFieldName(move)));
	ui.listMoves->setCurrentRow(ui.listMoves->model()->rowCount()-1);
}


void KnightsTour::onExit()
{
	int answer = QMessageBox::question(this, "Выход", "Вы уверены, что хотите выйти?", QMessageBox::Yes | QMessageBox::No);
	if(QMessageBox::Yes == answer) close();
}

void KnightsTour::onAbout()
{
	QString text =	"Курсовая работа по дисциплине \n\"Структуры и алгоритмы обработки данных\"\n"
		"Программная реализация алгоритма \"Ход конём\"\n"
		"Выполнил\tФилон Д. В.\n"
		"Группа\tВСМ-06-08";
	QMessageBox::about(this, "About", text);
}

void KnightsTour::onAboutQt()
{
	QMessageBox::aboutQt(this);
}

