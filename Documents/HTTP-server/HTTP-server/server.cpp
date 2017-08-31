#include "server.h"


Server::Server(QObject *parent) : QObject(parent)
{
    const int port = 8080;
    m_server = new QTcpServer(this);
    connect(m_server, SIGNAL(newConnection()),this,SLOT(Connect()));
    // in browser write 127.0.0:8080
    // listen 127.0.0:8080
    if (!m_server->listen(QHostAddress::Any,port)) {
        qDebug() << "\nServer could not start";
    }
    else {
        qDebug() <<"\nServer is waiting for a connection on port 8080";
    }
}

void Server::Connect()
{
    MySocket *my_socket = new MySocket(0,m_server->nextPendingConnection());
    QThread *new_thread = new QThread();
    connect(new_thread,SIGNAL(started()),my_socket,SLOT(Run()));
    connect(new_thread,SIGNAL(finished()),new_thread,SLOT(deleteLater()));
    connect(my_socket,SIGNAL(finish()),my_socket,SLOT(deleteLater()));
    new_thread->start();

}

QTcpServer *Server::getServer() const
{
    return m_server;
}

void Server::setServer(QTcpServer *server)
{
    m_server = server;
}

Server::~Server()
{
}
