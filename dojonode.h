#ifndef DOJONODE_H
#define DOJONODE_H

#include <QList>

#define TIME_CONST 30 //100 ms The time constants of different neurons typically range from 20 to 50 ms.
#define LENGHT_CONST 1 //0.5 mm Typical values for neuronal length constants range from 0.1 to 1.0 mm.
#define PUMP_CONST 1//1 mV per per 1 ms

class dojoSynapse;

//dojo Nodes
class dojoNode
{
protected:
    float Voltage;
    float RemainVoltage;

    QList <dojoSynapse*> Sources;
    QList <dojoSynapse*> Targets;

public:
    dojoNode();
    virtual void Process(float timeScale);
    virtual void AddTarget(dojoSynapse* target);
    virtual void AddSource(dojoSynapse* source);
    virtual dojoSynapse* GetSynapse(dojoNode* target);
};

class dojoSensor : public dojoNode {
public :
    dojoSensor();
    dojoSensor(float* data);
    void Process(float timeScale);

private:
    float* Data;

};
class dojoActuator : public dojoNode {
public :
    dojoActuator();
    dojoActuator(float* data);
    void Process(float timeScale);

private:
    float* Data;
};

//dojo Synapses
class dojoSynapse
{
public:
    dojoSynapse();
    dojoSynapse(dojoNode* source, dojoNode* target,float distance);

private :
    dojoNode* Source;
    dojoNode* Target;

    float Terminals;
    float Cleft;
    float Permability;
    float Distance;

public :
    float GetVoltage(float timeScale);
    void AP(dojoNode* node);
    float GetCleftValue();
    dojoNode* GetTarget();
};

#endif // DOJONODE_H
