#include "TaskFactory.h"
#include "BrandsTask.h"
#include "CarPicTask.h"
#include "CarListTask.h"
#include "PicLinkTask.h"

TaskFactory::TaskFactory(SendThread *pSendThread)
    :m_pTask(NULL),
     m_pSendThread(pSendThread)
{

}

TaskFactory::~TaskFactory()
{
    delete m_pTask;
    m_pTask = NULL;
}

ITask *TaskFactory::createTask(qint16 type)
{
    switch (type)
    {
    case 1:
        m_pTask = new BrandsTask(m_pSendThread);
        break;
    case 2:
        m_pTask = new CarListTask(m_pSendThread);
        break;
    case 3:
        m_pTask = new CarPicTask(m_pSendThread);
        break;
    case 4:
        m_pTask = new PicLinkTask(m_pSendThread);
        break;
    default:
        break;
    }

    return m_pTask;
}

void TaskFactory::recoverTask()
{
    delete m_pTask;
    m_pTask = NULL;
}


