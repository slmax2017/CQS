#ifndef NETCLIENT_H
#define NETCLIENT_H

#include <QString>
#include <QDebug>
#include "winsock2.h"
#define  cout qDebug() << "[ " << __FILE__ << ":" << __LINE__ << "]..."
#define DELPTR(x) if (x)\
                    { \
                        delete x;\
                        x = NULL; \
                    }

class NetClient
{
public:
    NetClient();
    ~NetClient();
private:
    SOCKET m_socket;
public:
    void connectHost(QString, qint16);
    SOCKET getSocket();
};

#endif // NETCLIENT_H
