#include "server.h"
#include <QDebug>
#include <QCoreApplication>
#include "mythread.h"
using namespace std;


Server::Server()
{
    const int kPort = 8080;
    // in browser write 127.0.0:8080
    // listen 127.0.0:8080
    if (!this->listen(QHostAddress::Any, kPort))
        qDebug() << "Server could not start";
    else
        qDebug() << "Server is waiting for a connection on port 8080";
}

void Server::incomingConnection(qintptr handle)
{
    MyThread *myThread = new MyThread(handle);
    connect(myThread, SIGNAL(finished()), myThread, SLOT(deleteLater()));
    myThread->start();
}

Server::~Server()
{
}
