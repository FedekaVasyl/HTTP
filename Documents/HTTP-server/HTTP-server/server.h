#ifndef SERVER_H
#define SERVER_H

#include <QCoreApplication>
#include <QNetworkInterface>
#include <iostream>
#include <QObject>
#include <QTcpSocket>
#include <QTcpServer>
#include <QDebug>
using namespace std;

class Server : public QObject
{
    Q_OBJECT
public slots:
    void Connect();
private:
    QTcpServer *server;
    QTcpSocket *socket;

public:
    explicit Server(QObject *parent = 0);
    virtual ~Server();
    QTcpServer* GetServer() const;
    void SetServer(QTcpServer *_server);
    QTcpSocket* GetSocket() const;
    void SetSocket(QTcpSocket *_socket);
};

#endif // SERVER_H
