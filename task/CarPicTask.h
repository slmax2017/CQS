#ifndef CARPICTASK_H
#define CARPICTASK_H

#include "ITask.h"

class CarPicTask : public ITask
{
public:
    CarPicTask(SendThread *pSendThread);
    ~CarPicTask();
protected:
    virtual void task(QString xml);
};


#endif // CARPICTASK_H
