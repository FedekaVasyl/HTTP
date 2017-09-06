#include "mysocket.h"
#include <QThread>
#include <QDebug>

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

MySocket::MySocket(QObject *parent, QTcpSocket *socket) : QObject(parent)
{
    this->socket = socket;
}

void MySocket::run()
{
    // forming the answer
    if (!readData()) {
        qDebug() << "Incorrect data!";
        closeSocket();
        return;
    }
    if (!writeToSocket()) {
        qDebug() << "Unable write to socket!";
        closeSocket();
        return;
    }
    closeSocket();
    emit this->finish();
}

bool MySocket::readData()
{
    QByteArray data;
    HttpRequestParser parser;
    HttpRequest new_request;
    QString length;
    int kWait = 1000;
    const char *blanck_line = "\r\n\r\n";
    while (!data.contains(blanck_line)) {  //read while not find blanck line
        if (!socket->waitForReadyRead(kWait)) {
            qDebug() << "Time is up!";
            return false;
        }
        data += socket->readAll();
    }
    new_request = parser.getHttpRequest(data);
    if (new_request.method.isNull()) {
       qDebug() << "Incorrect method!";
       return false;
    }
    if (new_request.uri.isNull()) {
        qDebug() << "Incorrect URI!";
        return false;
    }
    if (new_request.protocol_version.isNull()) {
        qDebug() << "Incorrect protocol version!";
        return false;
    }
    length = new_request.headers.value("Content-Length");
    if(!length.isNull()) {
        while (data.count() != length.toInt()) {
            if (!socket->waitForReadyRead(kWait)) {
                qDebug() << "Time is up!";
                return false;
            }

            data += socket->readAll();
        }
        new_request.body = data.right(length.toInt());
    } else {
        new_request.body = QByteArray();
    }
    setRequest(new_request);
    return true;
}

void MySocket::closeSocket()
{
    const int kWait = 1000;
    // close the socket
    connect(socket, SIGNAL(disconnected()), socket, SLOT(deleteLater()));
    socket->disconnectFromHost();
    if (socket->state() == QAbstractSocket::UnconnectedState
            || socket->waitForDisconnected(kWait)) {
        qDebug() << "Disconnected";
    } else {
        qDebug() << socket->error();
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
    socket->write(htmlPage.toStdString().c_str());
    socket->write(kHtmlEnd);
    socket->flush();
    return true;
}

MySocket::~MySocket()
{
}
