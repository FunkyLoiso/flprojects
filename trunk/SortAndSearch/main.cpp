#include "sortandsearch.h"
#include <QtGui/QApplication>
#include <QTextCodec>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("Windows-1251"));

	SortAndSearch w;
	w.show();
	return a.exec();
}
