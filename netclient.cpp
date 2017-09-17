#include "netclient.h"

NetClient::NetClient()
{
    WSADATA wsaData;
    if (WSAStartup(1, &wsaData))
    {
        cout << "网络模块初始化失败,请关闭程序";
    }

    m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (INVALID_SOCKET  == m_socket)
    {
        cout << "套接字创建失败";
    }
}

NetClient::~NetClient()
{
    closesocket(m_socket);
    WSACleanup();
}

void NetClient::connectHost(QString ip, qint16 port)
{
    SOCKADDR_IN addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(ip.toUtf8().constData());

    int len = sizeof(addr);

    int iret = connect(m_socket, (LPSOCKADDR)&addr, len);
    if (SOCKET_ERROR == iret)
    {
        cout << "服务器连接失败";
        return;
    }

    cout << "成功连接上服务器";
}

SOCKET NetClient::getSocket()
{
    return m_socket;
}

