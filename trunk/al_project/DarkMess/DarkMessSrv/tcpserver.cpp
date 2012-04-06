#include "tcpserver.h"

#include <QTcpSocket>
//----
#include <QDomDocument>
//----

TcpServer::TcpServer(QObject *parent) : QObject(parent),out(stdout)
{
    startTimer(REFRESH_INTERVAL_MS);


    if(srv.listen(QHostAddress::LocalHost, LISTEN_PORT))
        out << "Srv successfuly started on port" << LISTEN_PORT << endl;
}

void TcpServer::timerEvent(QTimerEvent */*event*/)
{
    // 1. Проверить новые подключения
    if(srv.hasPendingConnections())
    {
        //QTcpSocket* soc;
        soc_struct _soc;
        while(_soc.soc = srv.nextPendingConnection())
        {
            socs.push_back(_soc);
            out << "Added client, port" << _soc.soc->peerPort() << endl;
            out << _soc.soc->peerAddress().toString() << ":" << _soc.soc->peerPort() << endl;
        }
    }
    // 2. Получить новые сообщения
    for (int i = 0; i<socs.size(); i++)
    {
//        out << "lol2" <<endl;
        if (socs[i].soc->bytesAvailable())
        {
            QDomDocument xml;
            xml.setContent(socs[i].soc->readAll(),false);
//            QString str(socs[i].soc->readAll());
//            out <<  str << endl;
            out << xml.childNodes().at(0).attributes().item(0) << endl;
        }
    }
}
