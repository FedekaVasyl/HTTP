#ifndef TESTMYSOCKET_H
#define TESTMYSOCKET_H

#include <QObject>
#include <QTcpSocket>

class TestMySocket : public QObject
{
    Q_OBJECT

private slots:
    void testWriteToSocket();
    void testWriteToSocket_data();

    void testReadyRead();
    void testReadyRead_data();
private:
    void setSocketOption(QTcpSocket *socket, const QString &address, const int &port);
};

#endif // TESTMYSOCKET_H
