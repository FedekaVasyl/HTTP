#ifndef SERVER_H
#define SERVER_H

#include <QCoreApplication>
#include <QNetworkInterface>
#include <iostream>
#include <QTcpSocket>
#include <QTcpServer>
#include <QDebug>
#include <QString>
#include <QByteArray>
#include "mysocket.h"
using namespace std;

class Server : public QObject
{
    Q_OBJECT
public slots:
    void clientConnect();

private:
    QTcpServer *server_;

public:
    explicit Server(QObject *parent = 0);
    virtual ~Server();
    QTcpServer *getServer() const;
    void setServer(QTcpServer *server);



};

#endif // SERVER_H
