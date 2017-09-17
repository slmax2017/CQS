#ifndef IFACTORY_H
#define IFACTORY_H

#include "ITask.h"

class IFactory : public QObject
{
public:
    IFactory(){}
    virtual ~IFactory(){}
    virtual ITask *createTask(qint16 type) = 0;
protected:
    virtual void recoverTask() = 0;
};

#endif // IFACTORY_H
