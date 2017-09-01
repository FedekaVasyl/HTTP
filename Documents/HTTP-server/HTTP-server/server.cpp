#include "server.h"


Server::Server(QObject *parent) : QObject(parent)
{
    const int kPort = 8080;
    server_ = new QTcpServer(this);
    connect(server_, SIGNAL(newConnection()),
            this, SLOT(clientConnect()));
    // in browser write 127.0.0:8080
    // listen 127.0.0:8080
    if (!server_->listen(QHostAddress::Any, kPort)) {
        qDebug() << "\nServer could not start";
    } else {
        qDebug() << "\nServer is waiting for a connection on port 8080";
    }
}

void Server::clientConnect()
{
    MySocket *my_socket = new MySocket(NULL, server_->nextPendingConnection());
    QThread *new_thread = new QThread();
    connect(new_thread, SIGNAL(started()),
            my_socket, SLOT(run()));
    connect(new_thread, SIGNAL(finished()),
            new_thread, SLOT(deleteLater()));
    connect(my_socket, SIGNAL(finish()),
            my_socket, SLOT(deleteLater()));
    new_thread->start();
}

QTcpServer *Server::getServer() const
{
    return server_;
}

void Server::setServer(QTcpServer *server)
{
    server_ = server;
}

Server::~Server()
{
}
