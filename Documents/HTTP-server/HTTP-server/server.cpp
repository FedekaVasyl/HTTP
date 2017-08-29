#include "server.h"


Server::Server(QObject *parent) : QObject(parent)
{
    server = new QTcpServer(this);
    connect(server, SIGNAL(newConnection()),this,SLOT(Connect()));
    // in browser write 127.0.0:8080
    if(!server->listen(QHostAddress::Any,8080)) // listen 127.0.0:8080
    {
        cout<< "\nServer could not start";
    }
    else
    {
        cout<<"\nServer is waiting for a connection on port 8080";
    }
}

QTcpServer* Server::GetServer() const
{
    return this->server;
}

void Server::SetServer(QTcpServer *_server)
{
    this->server = _server;
}

QTcpSocket* Server::GetSocket() const
{
    return this->socket;
}

void Server::SetSocket(QTcpSocket *_socket)
{
    this->socket = _socket;
}

void Server::Connect()
{
    socket = server->nextPendingConnection();   // accept new connection
    while(!(socket->waitForReadyRead(100)));    // blocks calls until new data is available for reading

    char data[1000];
    memset(data,'\0',1000);
    int count = socket->read(data,1000);    // read data from socket
    // forming the answer
    socket->write("HTTP/1.1 200 OK\r\n");
    socket->write("Content-Type: text/html\r\n");
    socket->write("Connection: close\r\n");
    socket->write("Refresh: \r\n\r\n");
    socket->write("<!DOCTYPE html>\r\n");
    socket->write("<html><body>");
    socket->write(data);
    socket->write(" </body>\n</html>\n");
    socket->flush();
    // close the socket
    connect(socket, SIGNAL(disconnected()),socket, SLOT(deleteLater()));
    socket->disconnectFromHost();
}

Server::~Server()
{
    this->socket->close();
}
