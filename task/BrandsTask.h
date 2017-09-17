#ifndef BRANDSTASK_H
#define BRANDSTASK_H

#include "ITask.h"

class BrandsTask :public ITask
{
public:
    BrandsTask(SendThread *pSendThread);
    ~BrandsTask();
protected:
    virtual void task(QString xml);
public:

};


#endif
