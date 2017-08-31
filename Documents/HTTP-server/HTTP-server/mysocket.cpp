#include "mysocket.h"

QTcpSocket *MySocket::getSocket() const
{
    return m_socket;
}

void MySocket::setSocket(QTcpSocket *socket)
{
    m_socket = socket;
}

MySocket::MySocket(QObject *parent, QTcpSocket *socket) : QObject(parent)
{
    m_socket = socket;
}

void MySocket::Run()
{
    // forming the answer
    WriteToSocket(ReadData());
    CloseSocket();
    emit this->finish();
}

QString MySocket::ReadData()
{
    const int len = 1000;
    char *packet = new char[len];
    QString data;
    memset(packet,'\0',len);
    //m_socket->readLine(data,len);
    while(m_socket->read(packet,len)) {   // read data from socket
        data += packet;
        memset(packet,'\0',len);
    }
    delete packet;
    return data;
}

void MySocket::CloseSocket()
{
    m_socket->flush();
    // close the socket
    connect(m_socket, SIGNAL(disconnected()),m_socket, SLOT(deleteLater()));
    m_socket->disconnectFromHost();
    m_socket->close();
}

QString MySocket::ParseMessage(QString message)
{
    QString data;
    int i, j;
    const QString new_line = "<br>";
    for(i = 0, j = 0; i < message.length(); i++, j++) {
        if (message[i] == '\r' && message[i+1] == '\n') {
            data += new_line;
            j += 3;
            i++;
            continue;
        }
        data[j] = message[i];
    }
    return data;
}

void MySocket::WriteToSocket(QString data)
{
    const char *headers ="HTTP/1.1 200 OK\r\n"
                         "Content-Type: text/html\r\n"
                         "Connection: close\r\n\r\n";
    const char *html_begin = "<!DOCTYPE html>\r\n"
                             "<html><body>";
    const char *html_end = " </body>\n</html>\n";
    m_socket->write(headers);
    m_socket->write(html_begin);
    m_socket->write(ParseMessage(data).toStdString().c_str());
    m_socket->write(html_end);
}

MySocket::~MySocket()
{
}
