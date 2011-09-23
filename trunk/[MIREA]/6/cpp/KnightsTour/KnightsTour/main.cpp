#include "knightstour.h"
#include <QtGui/QApplication>
#include <QTextCodec>

int main(int argc, char *argv[])
{
	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("Windows-1251"));

	QApplication a(argc, argv);
	KnightsTour w;
	w.show();
	return a.exec();
}
