#ifndef MYSOCKET_H
#define MYSOCKET_H
#include <QThread>
#include <QObject>
#include <QTcpSocket>
#include <string>

class MySocket : public QObject
{
    Q_OBJECT
public slots:
    void Run();
signals:
    void finish();
private:
    QTcpSocket *m_socket;
public:
    explicit MySocket(QObject *parent = 0, QTcpSocket *socket = 0);
    virtual ~MySocket();

    QTcpSocket *getSocket() const;
    void setSocket(QTcpSocket *socket);

    QString ParseMessage(QString message);
    QString ReadData();
    void WriteToSocket(QString data);
    void CloseSocket();
};

#endif // MYSOCKET_H
