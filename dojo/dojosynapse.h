#ifndef DOJOSYNAPSE_H
#define DOJOSYNAPSE_H

#define LENGHT_CONST 1 //0.5 mm Typical values for neuronal length constants range from 0.1 to 1.0 mm.

class dojoNode;

class dojoSynapse
{
public:
    dojoSynapse();
    virtual float GetVoltage(float timeScale) = 0;
    virtual void AP(dojoNode* node, float timeScale) = 0;
    virtual dojoNode* GetTarget() = 0;
};
//dojo Synapses
class dojoSensorSynapse : public dojoSynapse
{
public :
    dojoSensorSynapse(float* source, dojoNode* target);

    float GetVoltage(float timeScale);
    void AP(dojoNode* node, float timeScale);
    dojoNode* GetTarget();

private :
    float* Source;
    dojoNode* Target;
};
class dojoMotorSynapse : public dojoSynapse{
public:
    dojoMotorSynapse(dojoNode* source, float* target, float singleOutputValue);

    float GetVoltage(float timeScale);
    void AP(dojoNode* node, float timeScale);
    dojoNode* GetTarget() ;
private :
    dojoNode* Source;
    float* Target;
    float Terminals;
};

class dojoChemicalSynapse : public dojoSynapse
{
public:
    dojoChemicalSynapse(dojoNode* source, dojoNode* target, float distance);

    float GetVoltage(float timeScale);
    void AP(dojoNode* node, float timeScale);
    dojoNode* GetTarget();

    float Cleft;

private :
    dojoNode* Source;
    dojoNode* Target;

    float Terminals;

    float Permability;
    float Distance;

    int timeSinceAp;
    bool isTested;
};

#endif // DOJOSYNAPSE_H
