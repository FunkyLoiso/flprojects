#ifndef TCPSERVER_H
#define TCPSERVER_H


#include <QObject>
#include <QTcpServer>

class TcpServer : public QObject
{
    Q_OBJECT

    static const int REFRESH_INTERVAL_MS = 500;
    static const int LISTEN_PORT = 1337;
public:
    explicit TcpServer(QObject *parent = 0);
    
signals:
    
public slots:

private:
    QTextStream in;
    QTextStream out;



    void timerEvent(QTimerEvent *event);
    
    struct soc_struct
    {
        QString name;
        QTcpSocket* soc;
    };

    QTcpServer srv;
    //QVector<QTcpSocket*> socs;
    QVector<soc_struct> socs;

};

#endif // TCPSERVER_H
