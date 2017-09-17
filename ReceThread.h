#ifndef RECETHREAD_H
#define RECETHREAD_H

#include <QThread>
#include <QString>
#include <QByteArray>
#include "netclient.h"
#include <string.h>

class ReceThread : public QObject
{
    Q_OBJECT
public:
    explicit ReceThread(NetClient *pNet, QObject *parent = 0);
    ~ReceThread();
signals:
    void sendToMain(const QString &xml);

public slots:
    void running();

private:
    SOCKET m_socket;
    char m_szBuff[1024 * 100];
    char m_szLen[7];
};

#endif // RECETHREAD_H
