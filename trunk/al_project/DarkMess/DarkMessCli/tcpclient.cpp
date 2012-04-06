#include "tcpclient.h"
#include <iostream>
#include <QTimer>
#include <QHostAddress>
#include <QTextStream>
//--
#include <QDomDocument>
//--

TcpClient::TcpClient(QObject *parent) : QObject(parent),in(stdin),out(stdout)
{
    QTimer::singleShot(1000, this, SLOT(consoleFunc()));
//  in = QTextStream(stdin);
}

void TcpClient::consoleFunc()
{
    out << "Connecting to server on port" << CONNECT_PORT << endl;

    out << "set name:" << flush;
    in >> name;

    soc.connectToHost(QHostAddress::LocalHost, CONNECT_PORT);
    soc.waitForConnected( 5000 );

    QAbstractSocket::SocketState state = soc.state();
    if(state != QAbstractSocket::ConnectedState)
    {
        out << "Error in connection, state:" << state << endl;
        return;
    }

//    while(soc.state() != QAbstractSocket::ConnectedState) sleep(1);

    out << "Connected!!" << endl;
    out <<  "soc name:" << name << endl;
    soc.write(FuckName());
    soc.flush();


    QString str;
    for  (int i=0;i<3;i++)
    {
        in >> str;
        out << "byte: " << soc.write(str.toAscii()) << endl;
        soc.flush();
    }
}

QByteArray TcpClient::FuckName()
{
    QDomDocument xml;
    QDomElement el = xml.createElement("system");
    el.setAttribute("name",name);
    xml.appendChild(el);
    //out << xml.toByteArray() << endl;
    return xml.toByteArray();
}


