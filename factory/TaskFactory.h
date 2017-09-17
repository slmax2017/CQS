#ifndef TASKFACTORY_H
#define TASKFACTORY_H

#include "IFactory.h"
#include "netclient.h"
#include "sendthread.h"

class TaskFactory : public IFactory
{
public:
    TaskFactory(SendThread *pSendThread);
    virtual ~TaskFactory();
public:
    virtual ITask *createTask(qint16 type);
    virtual void recoverTask();
private:
    SendThread *m_pSendThread;
    ITask *m_pTask;
};

#endif // TASKFACTORY_H
