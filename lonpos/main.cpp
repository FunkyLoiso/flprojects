#include "lonposwindow.h"
#include <QApplication>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LonposWindow w;
    w.show();
    
    return a.exec();
}
