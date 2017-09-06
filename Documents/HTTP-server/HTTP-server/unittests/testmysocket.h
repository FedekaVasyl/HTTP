#ifndef TESTMYSOCKET_H
#define TESTMYSOCKET_H

#include <QObject>

class TestMySocket : public QObject
{
    Q_OBJECT

private slots:
    void testWriteToSocket();
    void testWriteToSocket_data();
};

#endif // TESTMYSOCKET_H
