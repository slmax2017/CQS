#include "ReceThread.h"

ReceThread::ReceThread(NetClient *pNet, QObject *parent) :
    QObject(parent)
{
    m_socket = pNet->getSocket();
}

ReceThread::~ReceThread()
{
    closesocket(m_socket);
    WSACleanup();
}

void ReceThread::running()
{
    while (1)
    {
        int iIndex = 0;
        int iret = 0;
        memset(m_szBuff, 0, 1024 * 100);
        memset(m_szLen, 0, 7);

        while (iIndex < 6)
        {
            iret = recv(m_socket, m_szLen + iIndex, 6 - iIndex, 0);
            if (!iret)
            {
                closesocket(m_socket);
                WSACleanup();
                break;
            }
            else
            {
                iIndex += iret;
            }
        }

        QString tmp = QString(m_szLen);
        int len = tmp.toInt();
        len = len - 100000;
        iIndex = 0;
        iret = 0;

        while (iIndex < len)
        {
            iret = recv(m_socket, m_szBuff + iIndex, len - iIndex, 0);
            if (!iret)
            {
                closesocket(m_socket);
                WSACleanup();
                break;
            }
            else
            {
                iIndex += iret;
            }
        }


        QString xml = QString(m_szBuff);
        if (!xml.length()) continue;
        cout << xml.length();

        emit sendToMain(xml);
    }
}
