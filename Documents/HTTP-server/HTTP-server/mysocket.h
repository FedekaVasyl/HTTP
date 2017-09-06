#ifndef MYSOCKET_H
#define MYSOCKET_H

#include <QObject>
#include <QTcpSocket>
#include "httprequest.h"


class MySocket : public QObject
{
    Q_OBJECT
public slots:
    void run();
signals:
    void finish();
private:
    QTcpSocket *socket;
    HttpRequest request;
public:
    explicit MySocket(QObject *parent = NULL, QTcpSocket *socket = NULL);
    virtual ~MySocket();

    QTcpSocket *getSocket() const;
    void setSocket(QTcpSocket *socket);
    HttpRequest getRequest() const;
    void setRequest(HttpRequest request);

    bool readData();
    bool writeToSocket();
    void closeSocket();

    friend class TestMySocket;
};

#endif // MYSOCKET_H
