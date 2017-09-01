#ifndef MYSOCKET_H
#define MYSOCKET_H
#include <QThread>
#include <QObject>
#include <QTcpSocket>
#include <string>
#include "httprequest.h"


class MySocket : public QObject
{
    Q_OBJECT
public slots:
    void run();
signals:
    void finish();
private:
    QTcpSocket *socket_;

public:
    explicit MySocket(QObject *parent = 0, QTcpSocket *socket = 0);
    virtual ~MySocket();

    QTcpSocket *getSocket() const;
    void setSocket(QTcpSocket *socket);

    QByteArray parseMessage(QByteArray message);
    QByteArray readData();
    void writeToSocket(HttpRequest data);
    void closeSocket();
};

#endif // MYSOCKET_H
