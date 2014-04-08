#include "dojonode.h"
#include "math.h"

dojoSynapse::dojoSynapse(dojoNode* source, dojoNode* target, float distance){
    Source = source;
    Target = target;
    Distance = distance;

    Terminals = 100;
    Cleft = 0;
    Permability = 1;
}

float dojoSynapse::GetVoltage(float timeScale){
    //calc what is volatge injected by this synapse multiplied in ms
    float voltage = Cleft*Permability*timeScale;

    if(Cleft>0){
         //substract this voltage from Cleft if exist
        Cleft -= voltage;
        //synpase voltage will affect trigger area according to distance
        voltage = voltage*exp(-Distance/LENGHT_CONST);
    }
    else {
        //if no mediator in cleft, no synpase effect
        Cleft = 0;
        voltage = 0;
    }
    return voltage;
}

void dojoSynapse::AP(dojoNode* node){
    //if it is Presynapse
    if(node == Source)
        //eject Mediator
        Cleft = Terminals;
    //if it is Postsynapse
    else if (node == Target){
        //change permability (Hebb rule)
        Permability = 1/(exp(5-Cleft));
    }
}

dojoNode::dojoNode(){
    Voltage = 0;
    RemainVoltage = 0;
}

void dojoNode::Process(float timeScale){
    //Collect voltage from all source Synapses
    float synapseVoltage = 0;
    for (int i = 0; i < Sources.size(); ++i) {
        synapseVoltage += Sources[i]->GetVoltage(timeScale);
    }
    //calc adding voltage depends on current synaptic and previously remained
    float addedVoltage = (RemainVoltage+synapseVoltage)*(1-exp(-timeScale/TIME_CONST));
    //calc current membrane voltage
    Voltage += addedVoltage;

    //check if threshold
    if(Voltage>100) {
        Voltage = 0;
        RemainVoltage = 0;
        //Generate AP
        for (int i = 0; i < Targets.size(); ++i) {
            Targets[i]->AP(this);
        }
        for (int i = 0; i < Sources.size(); ++i) {
            Sources[i]->AP(this);
        }
    }
    //not AP
    else {
        if(Voltage>0) RemainVoltage -= addedVoltage + timeScale * PUMP_CONST;
        else RemainVoltage -= addedVoltage - timeScale * PUMP_CONST;
    }
}

void dojoNode::AddTarget(dojoSynapse* target){
    Targets<<target;
}

void dojoNode::AddSource(dojoSynapse* source){
    Sources<<source;
}
dojoSensor::dojoSensor(float *data){
    Data = data;
}
void dojoSensor::Process(float timeScale){
    //calc current membrane voltage
    Voltage += *Data;

    //check if threshold
    if(Voltage>100) {
        Voltage = 0;
        RemainVoltage = 0;
        //Generate AP
        for (int i = 0; i < Targets.size(); ++i) {
            Targets[i]->AP(this);
        }
    }
}
dojoActuator::dojoActuator(float* data){
    Data = data;
}
void dojoActuator::Process(float timeScale){
    //Collect voltage from all source Synapses
    float synapseVoltage = 0;
    for (int i = 0; i < Sources.size(); ++i) {
        synapseVoltage += Sources[i]->GetVoltage(timeScale);
    }
    //calc adding voltage depends on current synaptic and previously remained
    float addedVoltage = (RemainVoltage+synapseVoltage)*(1-exp(-timeScale/TIME_CONST));
    //calc current membrane voltage
    Voltage += addedVoltage;

    //check if threshold
    if(Voltage>100) {
        Voltage = 0;
        RemainVoltage = 0;
        //Generate AP

        *Data += 100;

        for (int i = 0; i < Sources.size(); ++i) {
            Sources[i]->AP(this);
        }
    }
    //not AP
    else {
        if(Voltage>0) RemainVoltage -= addedVoltage + timeScale * PUMP_CONST;
        else RemainVoltage -= addedVoltage - timeScale * PUMP_CONST;
    }
}
