#include "tcpclient.h"

#include <QTimer>
#include <QHostAddress>
#include <QTextStream>

TcpClient::TcpClient(QObject *parent) : QObject(parent)
{
    QTimer::singleShot(1000, this, SLOT(consoleFunc()));
}

void TcpClient::consoleFunc()
{
    qDebug() << "Connecting to server on port" << CONNECT_PORT;
    soc.connectToHost(QHostAddress::LocalHost, CONNECT_PORT);
    soc.waitForConnected( 5000 );

    QAbstractSocket::SocketState state = soc.state();
    if(state != QAbstractSocket::ConnectedState)
    {
        qDebug() << "Error in connection, state:" << state;
        return;
    }

//    while(soc.state() != QAbstractSocket::ConnectedState) sleep(1);

    qDebug() << "Connected!!";

    QTextStream in(stdin);
    QString str;
    for  (int i=0;i<3;i++)
    {
        in >> str;
        qDebug() << "byte: " << soc.write(str.toAscii());
        soc.flush();
    }
}
