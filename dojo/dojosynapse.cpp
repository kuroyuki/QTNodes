#include "dojosynapse.h"
#include "math.h"

dojoSynapse::dojoSynapse(){

}
/*
 *Sensor synapses - use only for input from outside
 */
dojoSensorSynapse::dojoSensorSynapse(float *source, dojoNode *target){
    Target = target;
    Source = source;
}
float dojoSensorSynapse::GetVoltage(float timeScale){
    return *Source;
}
void dojoSensorSynapse::AP(dojoNode *node, float timeScale){
    return;
}
dojoNode* dojoSensorSynapse::GetTarget(){
    return Target;
}

/*
 *Motor synapses - used only for outpud to external float variable
 */
dojoMotorSynapse::dojoMotorSynapse(dojoNode* source, float* target, float singleOutputValue){
    Source = source;
    Target = target;
    Terminals = singleOutputValue;
}

float dojoMotorSynapse::GetVoltage(float timeScale){
    return *Target;
}

void dojoMotorSynapse::AP(dojoNode* node, float timeScale){
    *Target += Terminals;
}

dojoNode* dojoMotorSynapse::GetTarget(){
    return Source;
}

/*
 *Normal Chemical synapses - common for node interconnection in the network
 */
dojoChemicalSynapse::dojoChemicalSynapse(dojoNode* source, dojoNode* target, float distance)
{
    Source = source;
    Target = target;
    Distance = distance;

    Terminals = 100;
    Cleft = 0;
    Permability = 5;
}

float dojoChemicalSynapse::GetVoltage(float timeScale){
    float voltage = 0;
    //calc what is volatge injected by this synapse multiplied in ms
    if(Cleft>0){
        //synpase voltage will affect trigger area according to distance
        voltage = Cleft*Permability*exp(-Distance/LENGHT_CONST);
    }
    //remove all transmitter from cleft
    Cleft = 0;

    return voltage;
}

void dojoChemicalSynapse::AP(dojoNode* node, float timeScale){
    //if it is Presynapse
    if(node == Source){
        //eject Mediator
        Cleft = Terminals;
        /*
        //change Terminals count for next release depends on timeScale in ms
        //e.g. if it less than 50 ms it will decreased, if it more than 100 ms it will be decreased but more slowly
        if(timeScale < 75)
            Terminals
            */
    }
    //if it is Postsynapse
    else if (node == Target){
        //change permability (Hebb rule)
        Permability = 5;//1/(exp(5-Cleft));
    }
}
dojoNode* dojoChemicalSynapse::GetTarget(){
    return Target;
}
