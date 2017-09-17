#ifndef PICLINKTASK_H
#define PICLINKTASK_H

#include "ITask.h"

class PicLinkTask : public ITask
{
public:
    PicLinkTask(SendThread *pSendThread);
    ~PicLinkTask(){}
protected:
    virtual void task(QString xml);
};

#endif // PICLINKTASK_H
