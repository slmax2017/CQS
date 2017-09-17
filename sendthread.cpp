#include "sendthread.h"
#include <QDebug>

SendThread::SendThread(NetClient *pNet, QObject *parent) :
    QObject(parent)
{
    m_socket = pNet->getSocket();
    m_lstData.clear();
}

SendThread::~SendThread()
{
    m_lstData.clear();
}

void SendThread::running()
{
    while (1)
    {
        if (m_lstData.empty())
        {
            QThread::sleep(1);
            continue;
        }

        QString xmlStr = m_lstData.front();
        m_lstData.pop_front();
        emit sendToMain(xmlStr);


        send(m_socket, xmlStr.toUtf8().data(), xmlStr.toUtf8().length(), 0);
    }
}

void SendThread::pushData(QString xmlStr)
{
    m_lstData.push_back(xmlStr);
}

