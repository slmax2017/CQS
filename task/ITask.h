#ifndef ITASK_H
#define ITASK_H

#include <QString>
#include "sendthread.h"
#include "tinystr.h"
#include "tinyxml.h"

class ITask
{
public:
    ITask(SendThread *pSendThread):m_pSendThread(pSendThread){}
    virtual ~ITask(){}
public:
    virtual void task(QString xml) = 0;
protected:
    SendThread *m_pSendThread;

};

#endif
