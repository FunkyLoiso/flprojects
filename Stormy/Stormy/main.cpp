#include "stormy.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Stormy w;
	w.show();
	return a.exec();
}
