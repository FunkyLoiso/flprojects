#include "lonpos_f_l.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Lonpos_F_L w;
	w.show();
	return a.exec();
}
