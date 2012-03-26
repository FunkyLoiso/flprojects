#include <QtCore/QCoreApplication>

#include "tcpclient.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    new TcpClient(&a);
    
    return a.exec();
}
