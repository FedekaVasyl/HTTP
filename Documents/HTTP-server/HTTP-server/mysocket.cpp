#include "mysocket.h"

QTcpSocket *MySocket::getSocket() const
{
    return socket_;
}

void MySocket::setSocket(QTcpSocket *socket)
{
    socket_ = socket;
}

MySocket::MySocket(QObject *parent, QTcpSocket *socket) : QObject(parent)
{
    socket_ = socket;
}

void MySocket::run()
{
    // forming the answer
    HttpRequest request(readData());
    writeToSocket(request);
    closeSocket();
    emit this->finish();
}

QByteArray MySocket::readData()
{
    const int kLength = 1000;
    const int kWait = 1000;
    char packet[kLength];
    QByteArray data;
    memset(packet, '\0', kLength);
    socket_->waitForReadyRead(kWait);
    while (socket_->read(packet, kLength)) {   // read data from socket
        data += packet;
        memset(packet, '\0', kLength);
    }
    return data;
}

void MySocket::closeSocket()
{
    socket_->flush();
    // close the socket
    connect(socket_, SIGNAL(disconnected()),
            socket_, SLOT(deleteLater()));
    socket_->disconnectFromHost();
    socket_->close();
}

QByteArray MySocket::parseMessage(QByteArray message)
{
    QByteArray data;
    int i, j;
    const QString kNewLine = "<br>";
    for (i = 0, j = 0; i < message.length(); i++, j++) {
        if (message[i] == '\r' && message[i + 1] == '\n') {
            data += kNewLine;
            j += 3;
            i++;
            continue;
        }
        data[j] = message[i];
    }
    return data;
}

void MySocket::writeToSocket(HttpRequest data)
{
    const char *kHeaders ="HTTP/1.1 200 OK\r\n"
                         "Content-Type: text/html\r\n"
                         "Connection: close\r\n\r\n";
    const char *kHtmlBegin = "<!DOCTYPE html>\r\n"
                             "<html><body>";
    const char *kHtmlEnd = " </body>\n</html>\n";
    socket_->write(kHeaders);
    socket_->write(kHtmlBegin);
    socket_->write(data.formAsHtmlPage().toStdString().c_str());
    socket_->write(kHtmlEnd);
}

MySocket::~MySocket()
{
}
