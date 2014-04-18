#include "dojonode.h"
#include "math.h"

dojoNode::dojoNode(){
    Voltage = 0;
}

void dojoNode::Process(float timeScale){
    //Collect voltage from all source Synapses
    float synapseVoltage = 0;
    for (int i = 0; i < Sources.size(); ++i) {
        synapseVoltage += Sources[i]->GetVoltage(timeScale);
    }
    //calc adding voltage depends on current synaptic and previously remained
    float addedVoltage = synapseVoltage*(1-exp(-timeScale/TIME_CONST));
    //calc current membrane voltage
    Voltage += addedVoltage;

    //check if threshold
    if(Voltage>=100) {
        Voltage = 0;

        //Generate AP
        for (int i = 0; i < Targets.size(); ++i) {
            Targets[i]->AP(this, timeScale);
        }
        for (int i = 0; i < Sources.size(); ++i) {
            Sources[i]->AP(this, timeScale);
        }
    }
    //not AP
    else {
        if(Voltage>0) Voltage -= timeScale * PUMP_CONST;
        else Voltage = timeScale * PUMP_CONST;
    }
}

void dojoNode::AddTarget(dojoSynapse* target){
    Targets<<target;
}

void dojoNode::AddSource(dojoSynapse* source){
    Sources<<source;
}

dojoSynapse* dojoNode::GetSynapse(dojoNode* target){
    for (int i = 0; i < Targets.size(); ++i) {
        if (Targets.at(i)->GetTarget() == target)
            return Targets.at(i);
    }
    return 0;
}
