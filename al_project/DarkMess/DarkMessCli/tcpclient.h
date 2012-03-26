#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>

#include <QTcpSocket>

class TcpClient : public QObject
{
    Q_OBJECT

    static const int REFRESH_INTERVAL_MS = 500;
    static const int CONNECT_PORT = 1337;
public:
    explicit TcpClient(QObject *parent = 0);

signals:
    
private slots:
     void consoleFunc();

private:
    QTcpSocket soc;
    
};

#endif // TCPCLIENT_H
