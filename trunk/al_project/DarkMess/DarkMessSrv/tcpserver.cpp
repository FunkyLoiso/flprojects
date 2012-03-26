#include "tcpserver.h"

#include <QTcpSocket>


TcpServer::TcpServer(QObject *parent) : QObject(parent)
{
    startTimer(REFRESH_INTERVAL_MS);


    if(srv.listen(QHostAddress::LocalHost, LISTEN_PORT))
        qDebug() << "Srv successfuly started on port" << LISTEN_PORT;
}

void TcpServer::timerEvent(QTimerEvent */*event*/)
{
    // 1. Проверить новые подключения
    if(srv.hasPendingConnections())
    {
        QTcpSocket* soc;
        while(soc = srv.nextPendingConnection())
        {
            socs.push_back(soc);
            qDebug() << "Added client, port" << soc->peerPort();
        }
    }
    // 2. Получить новые сообщения
    for (int i = 0; i<socs.size(); i++)
    {
        if (socs[i]->bytesAvailable())
        {
            QString str(socs[i]->readAll());
            qDebug() <<  str;
        }
    }
}
