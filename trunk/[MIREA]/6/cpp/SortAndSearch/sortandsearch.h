#ifndef SORTANDSEARCH_H
#define SORTANDSEARCH_H

#include <QtGui/QMainWindow>
#include "ui_sortandsearch.h"

#include "ArrayModel.h"

class SortAndSearch : public QMainWindow
{
	Q_OBJECT

public:
	SortAndSearch(QWidget *parent = 0, Qt::WFlags flags = 0);
	~SortAndSearch();

private:
	Ui::SortAndSearchClass ui;
	QVector<int> m_array;			///< ������
	IntArrayModel m_arrayModel;	///< ������ ��� ��������� ������ ���������

	void resizeArray(int newSize);	///< �������� ������ �������

private slots:
	void onSetArraySize();
	void onRandomizeElements();
	void onDoSort();
	void onDoSearch();
	void onExit();
	void onAbout();
	void onAboutQt();
};

#endif // SORTANDSEARCH_H
