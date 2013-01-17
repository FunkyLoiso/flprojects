#include <QtGui/QApplication>
#include "lonposwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LonposWindow w;
    w.show();
    
    return a.exec();
}
