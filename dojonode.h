#ifndef DOJONODE_H
#define DOJONODE_H

#include <QList>

#define TIME_CONST 30 //100 ms The time constants of different neurons typically range from 20 to 50 ms.
#define PUMP_CONST 1//1 mV per per 1 ms

class dojoNode;

#include "dojosynapse.h"

//dojo Nodes
class dojoNode
{
protected:
    float Voltage;

    QList <dojoSynapse*> Sources;
    QList <dojoSynapse*> Targets;

public:
    dojoNode();
    virtual void Process(float timeScale);
    virtual void AddTarget(dojoSynapse* target);
    virtual void AddSource(dojoSynapse* source);
    virtual dojoSynapse* GetSynapse(dojoNode* target);
};
#endif // DOJONODE_H
