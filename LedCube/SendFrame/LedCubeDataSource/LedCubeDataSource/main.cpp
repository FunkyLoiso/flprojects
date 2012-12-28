#include "ledcubedatasource.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	LedCubeDataSource w;
	w.show();
	return a.exec();
}
