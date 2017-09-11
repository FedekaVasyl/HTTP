#include "mysocket.h"
#include <QDebug>
#include <QTimer>

MySocket::MySocket(QObject *parent, QTcpSocket *socket) : QObject(parent)
{
    this->socket = socket;
    connect(this->socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    QTimer::singleShot(10000, this, SLOT(timeIsUp()));
}

QTcpSocket *MySocket::getSocket() const
{
    return socket;
}

void MySocket::setSocket(QTcpSocket *socket)
{
    this->socket = socket;
}

HttpRequest MySocket::getRequest() const
{
    return request;
}

void MySocket::setRequest(HttpRequest request)
{
    this->request = request;
}

void MySocket::readyRead()
{
    const char *blanckLine = "\r\n\r\n";
    thread_local QByteArray data;
    thread_local bool isParsed = false;
    HttpRequestParser parser;
    HttpRequest newRequest;
    QString length;
    data += socket->readAll();
    if (data.contains(blanckLine)) {
        if (!isParsed) {
            newRequest = parser.getHttpRequest(data);
            if (newRequest.method.isNull()) {
               qDebug() << "Incorrect method!";
               closeSocket();
               return;
            }
            if (newRequest.uri.isNull()) {
                qDebug() << "Incorrect URI!";
                closeSocket();
                return;
            }
            if (newRequest.protocolVersion.isNull()) {
                qDebug() << "Incorrect protocol version!";
                closeSocket();
                return;
            }
            isParsed = true;
            request = newRequest;
        }
        length = request.headers.value("Content-Length");
        if (length.isNull()) {
            request.body = QByteArray();
            if (!writeToSocket()) {
                qDebug() << "Unable write to socket!";
                closeSocket();
                return;
            }
        } else {
            if (length.toInt() + parser.requestMessageHeaderSize(request) == data.size()) {
                request.body = data.right(length.toInt());
                if (!writeToSocket()) {
                    qDebug() << "Unable write to socket!";
                    closeSocket();
                    return;
                }
            }
        }
    }
}

void MySocket::timeIsUp()
{
    qDebug() << "Connection time has passed";
    closeSocket();
}

void MySocket::closeSocket()
{
    const int kWait = 1000;
    HttpRequest nullRequest;
    request = nullRequest;
    connect(socket, SIGNAL(disconnected()), socket, SLOT(deleteLater()));
    socket->disconnectFromHost();
    if (socket->state() == QAbstractSocket::UnconnectedState
            || socket->waitForDisconnected(kWait)) {
        qDebug() << "Disconnected";
    } else {
        qDebug() << socket->error();
        emit this->finish();
    }
    socket->close();
}

bool MySocket::writeToSocket()
{
    HttpRequestParser parser;
    QByteArray htmlPage = parser.formAsHtmlPage(this->request);
    if (htmlPage.isNull()) {
        qDebug() << "Incorrect data!";
        return false;
    }
    if (!socket->isOpen() || !socket->isValid()) {
        qDebug() << "Socket is closed or invalid";
        return false;
    }
    const char *kHeaders ="HTTP/1.1 200 OK\r\n"
                         "Content-Type: text/html\r\n"
                         "Connection: close\r\n\r\n";
    const char *kHtmlBegin = "<!DOCTYPE html>\r\n"
                             "<html><body>";
    const char *kHtmlEnd = " </body>\n</html>\n";
    socket->write(kHeaders);
    socket->write(kHtmlBegin);
    socket->write(htmlPage);
    socket->write(kHtmlEnd);
    socket->flush();
    closeSocket();
    return true;
}

MySocket::~MySocket()
{
}
