#ifndef SENDTHREAD_H
#define SENDTHREAD_H

#include <QThread>
#include <QString>
#include <list>
#include "netclient.h"
using namespace std;

class SendThread : public QObject
{
    Q_OBJECT
public:
    explicit SendThread(NetClient *pNet, QObject *parent = 0);
    ~SendThread();

signals:
    void sendToMain(const QString &x);
public slots:
    void running();
    void pushData(QString xmlStr);
private:
    list<QString> m_lstData;
    SOCKET m_socket;
};

#endif // SENDTHREAD_H
