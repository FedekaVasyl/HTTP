#ifndef MYTHREAD_H
#define MYTHREAD_H
#include <QThread>
#include <QObject>
#include <QTcpSocket>
#include "mysocket.h"

class MyThread : public QThread
{
    Q_OBJECT

public:
    MyThread(qintptr descriptor);
    virtual ~MyThread();

protected:
    void run() Q_DECL_OVERRIDE;

private slots:
    void onDisconnected();

private:
    qintptr descriptor;
    MySocket *mySocket;
};

#endif // MYTHREAD_H
