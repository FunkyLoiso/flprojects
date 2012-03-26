#include <QtCore/QCoreApplication>

#include "tcpserver.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    new TcpServer(&a);
    
    int code =  a.exec();
    return code;
}
