
#include <QtCore/QCoreApplication>
#include "LayoutIndicatorProvider.h"

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	LayoutIndicatorProvider lip;

	return a.exec();
}
