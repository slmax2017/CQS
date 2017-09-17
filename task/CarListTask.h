#ifndef CARLISTTASK_H
#define CARLISTTASK_H

#include "ITask.h"

class CarListTask : public ITask
{
public:
    CarListTask(SendThread *pSendThread);
    ~CarListTask();
protected:
    virtual void task(QString xml);
};

#endif // CARLISTTASK_H
