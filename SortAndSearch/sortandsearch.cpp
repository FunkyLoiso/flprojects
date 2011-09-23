#include "sortandsearch.h"
#include <QMessageBox>
#include "algorithms.h"

#include "windows.h"//для таймера производительности

SortAndSearch::SortAndSearch(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags), m_arrayModel(m_array)
{
	ui.setupUi(this);
	ui.listElements->setModel(&m_arrayModel);

	QObject::connect(ui.buttonSetArraySize, SIGNAL(clicked()), this, SLOT(onSetArraySize()));
	QObject::connect(ui.buttonRandomizeElements, SIGNAL(clicked()), this, SLOT(onRandomizeElements()));
	QObject::connect(ui.buttonDoSort, SIGNAL(clicked()), this, SLOT(onDoSort()));
	QObject::connect(ui.buttonDoSearch, SIGNAL(clicked()), this, SLOT(onDoSearch()));
	QObject::connect(ui.buttonExit, SIGNAL(clicked()), this, SLOT(onExit()));
	QObject::connect(ui.actionAbout, SIGNAL(triggered()), this, SLOT(onAbout()));
	QObject::connect(ui.actionAbout_Qt, SIGNAL(triggered()), this, SLOT(onAboutQt()));
}

SortAndSearch::~SortAndSearch()
{

}

void SortAndSearch::onSetArraySize()
{
	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
	QString oldText = ui.buttonSetArraySize->text();
	ui.buttonSetArraySize->setText("идёт выполнение...");

	resizeArray(ui.spinboxArraySize->value());

	QApplication::restoreOverrideCursor();
	ui.buttonSetArraySize->setText(oldText);
}

void SortAndSearch::resizeArray( int newSize )
{
	try
	{
		m_array.resize(newSize);
	}
	catch(...)
	{
		QMessageBox::critical(this, "Фатальная ошибка", QString("Невозможно выделить память под список. Запрошенное число элементов: %1").arg(newSize), QMessageBox::Ok);
		resizeArray(0);
	}

	m_arrayModel.UpdateRowCount();//обновим список
	ui.editCurArraySize->setText( QString::number(m_array.size()) );//выведем новый размер
}

void SortAndSearch::onRandomizeElements()
{
	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
	QString oldText = ui.buttonRandomizeElements->text();
	ui.buttonRandomizeElements->setText("идёт выполнение...");


	for(QVector<int>::iterator i = m_array.begin(); i != m_array.end(); ++i) *i = ( qrand()*0x7fff + qrand() )*( qrand()%2 ? -1 : 1 );
	m_arrayModel.UpdateAll();

	QApplication::restoreOverrideCursor();
	ui.buttonRandomizeElements->setText(oldText);
}

void SortAndSearch::onDoSort()
{
	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
	QString oldText = ui.buttonDoSort->text();
	ui.buttonDoSort->setText("идёт выполнение...");

	comps = 0;
	moves = 0;
	LARGE_INTEGER beg, end, freq;
	QueryPerformanceFrequency(&freq);
	freq.QuadPart /= 1000;//теперь это циклы за мс
	QueryPerformanceCounter(&beg);
	heapSort(m_array, m_array.size());
	QueryPerformanceCounter(&end);
	m_arrayModel.UpdateAll();

	ui.editCnSort->setText(QString::number(comps));
	ui.editMnSort->setText(QString::number(moves));

	unsigned int time_ms = (end.QuadPart - beg.QuadPart)/freq.QuadPart;
	ui.editSortTime->setText(QString("%1 мс").arg(time_ms));

	QApplication::restoreOverrideCursor();
	ui.buttonDoSort->setText(oldText);
}

void SortAndSearch::onDoSearch()
{
	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
	QString oldText = ui.buttonDoSearch->text();
	ui.buttonDoSearch->setText("идёт выполнение...");

	comps = 0;
	moves = 0;
	LARGE_INTEGER beg, end, freq;
	QueryPerformanceFrequency(&freq);
	freq.QuadPart /= 1000000;//теперь это циклы за нс

	int searchVal = ui.spinboxSearchVal->value();

	QueryPerformanceCounter(&beg);
	int idx = interpolationSearch(m_array, m_array.size(), searchVal);
	QueryPerformanceCounter(&end);

	QString result = idx != -1 ? QString::number(idx) : QString("Не найден, либо массив не отсортирован");
	ui.editFoundIndex->setText(result);

	ui.listElements->setCurrentIndex(m_arrayModel.index(idx));

	ui.editCnSearch->setText(QString::number(comps));
	ui.editMnSearch->setText(QString::number(moves));

	unsigned int time_ms = (end.QuadPart - beg.QuadPart)/freq.QuadPart;
	ui.editSearchTime->setText(QString("%1 нс").arg(time_ms));

	QApplication::restoreOverrideCursor();
	ui.buttonDoSearch->setText(oldText);
}

void SortAndSearch::onExit()
{
	int answer = QMessageBox::question(this, "Выход", "Вы уверены, что хотите выйти?", QMessageBox::Yes | QMessageBox::No);
	if(QMessageBox::Yes == answer) close();
}

void SortAndSearch::onAbout()
{
	QString text =	"Лабораторные работы 3 и 4 по дисциплине \n\"Структуры и алгоритмы обработки данных\"\n"
					"Выполнил\tФилон Д. В.\n"
					"Группа\tВСМ-06-08";
	QMessageBox::about(this, "About", text);
}

void SortAndSearch::onAboutQt()
{
	QMessageBox::aboutQt(this);
}

