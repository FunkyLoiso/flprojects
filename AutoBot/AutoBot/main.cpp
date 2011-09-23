#include "MainWindow.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	AutoBotMainWindow w;
	w.show();
	return a.exec();
}
