#include "server.h"
#include <QThread>
#include <QDebug>
#include <QString>
#include <QCoreApplication>
#include <QNetworkInterface>
#include <iostream>
using namespace std;


Server::Server(QObject *parent) : QObject(parent)
{
    const int kPort = 8080;
    server = new (nothrow) QTcpServer(this);
    if (!server) {
        qDebug() << "Could not create new server";
    } else {
        connect(server, SIGNAL(newConnection()), this, SLOT(clientConnect()));
        // in browser write 127.0.0:8080
        // listen 127.0.0:8080
        if (!server->listen(QHostAddress::Any, kPort))
            qDebug() << "Server could not start";
        else
            qDebug() << "Server is waiting for a connection on port 8080";
    }
}

void Server::clientConnect()
{
    MySocket *my_socket = new (nothrow) MySocket(NULL, server->nextPendingConnection());
    if (!my_socket) {
        qDebug() << "Could not create new MySocket";
        return;
    }
    QThread *new_thread = new (nothrow) QThread();
    if (!new_thread) {
        qDebug() << "Could not create new thread";
        return;
    }
    connect(new_thread, SIGNAL(started()), my_socket, SLOT(run()));
    connect(new_thread, SIGNAL(finished()), new_thread, SLOT(deleteLater()));
    connect(my_socket, SIGNAL(finish()), my_socket, SLOT(deleteLater()));
    new_thread->start();
}

QTcpServer *Server::getServer() const
{
    return server;
}

void Server::setServer(QTcpServer *server)
{
    this->server = server;
}

Server::~Server()
{
}
