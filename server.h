#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>

class Server : public QTcpServer
{
    Q_OBJECT

public:
    explicit Server();
    virtual ~Server();

protected:
    virtual void incomingConnection(qintptr handle) Q_DECL_OVERRIDE;
};

#endif // SERVER_H
