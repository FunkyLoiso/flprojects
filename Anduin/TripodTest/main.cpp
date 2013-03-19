#include "tripodtest.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	TripodTest w;
	w.show();
	return a.exec();
}
