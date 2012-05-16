#include "romancalc.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	RomanCalc w;
	w.show();
	return a.exec();
}
