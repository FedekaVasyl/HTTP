#ifndef SERVER_H
#define SERVER_H

#include <QTcpSocket>
#include <QTcpServer>
#include <QByteArray>
#include "mysocket.h"

class Server : public QObject
{
    Q_OBJECT
public slots:
    void clientConnect();

private:
    QTcpServer *server;

public:
    explicit Server(QObject *parent = 0);
    virtual ~Server();
    QTcpServer *getServer() const;
    void setServer(QTcpServer *server);
};

#endif // SERVER_H
