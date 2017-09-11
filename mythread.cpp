#include "mythread.h"
#include "mysocket.h"

MyThread::MyThread(qintptr descriptor)
{
    this->descriptor = descriptor;
}

void MyThread::run()
{
    QTcpSocket *socket = new QTcpSocket();
    socket->setSocketDescriptor(descriptor);
    mySocket = new MySocket(NULL, socket);
    connect(mySocket->getSocket(), SIGNAL(disconnected()), this, SLOT(onDisconnected()));
    connect(mySocket, SIGNAL(finish()), this, SLOT(onDisconnected()));
    exec();
}

void MyThread::onDisconnected()
{
    this->quit();
}

MyThread::~MyThread()
{
    delete this->mySocket;
}
