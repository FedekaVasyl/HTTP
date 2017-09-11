#ifndef MYSOCKET_H
#define MYSOCKET_H

#include <QObject>
#include <QTcpSocket>
#include "httprequest.h"


class MySocket : public QObject
{
    Q_OBJECT

public:
    explicit MySocket(QObject *parent = NULL, QTcpSocket *socket = NULL);
    virtual ~MySocket();

    QTcpSocket *getSocket() const;
    void setSocket(QTcpSocket *socket);
    HttpRequest getRequest() const;
    void setRequest(HttpRequest request);

    bool writeToSocket();
    void closeSocket();

    friend class TestMySocket;
private slots:
    void readyRead();
    void timeIsUp();
signals:
    void finish();
private:
    QTcpSocket *socket;
    HttpRequest request;
};

#endif // MYSOCKET_H
