#include "stormy.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Stormy w;
	w.show();
	return a.exec();
}
