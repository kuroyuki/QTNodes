#include "dojonetwork.h"
#include <QTimer>
#include <QJsonObject>

#include "math.h"

dojoNetwork::dojoNetwork(QObject *parent) :
    QObject(parent)
{
    counter = 0;

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(Process()));
    timer->start(TIME_SCALE);
}

void dojoNetwork::Process(){
   counter++;

   foreach (dojoNode* node, NodeTable)
       node->Process(TIME_SCALE);
}
/* dojoEvents :
 * 1 - node create
 * 2 - sensor create
 * 3 - act create
 * 4 - bind nodes
 **/

void dojoNetwork::CreateNode(int x, int y){
    QString string =  QString::number(x)+','+QString::number(y);
    NodeTable[string] = new dojoNode();

    QString event;
    event += "1," + QString::number(x)+','+QString::number(y);

    emit dojoEvent(event);
}
void dojoNetwork::CreateSensor(float* data, int x, int y){
    QString string =  QString::number(x)+','+QString::number(y);
    NodeTable[string] = new dojoSensor(data);

    QString event;
    event += "2," + QString::number(x)+','+QString::number(y);

    emit dojoEvent(event);
}

void dojoNetwork::CreateActuator(float* data, int x, int y){
    QString string =  QString::number(x)+','+QString::number(y);
    NodeTable[string] = new dojoActuator(data);

    QString event;
    event += "3," + QString::number(x)+','+QString::number(y);

    emit dojoEvent(event);
}

void dojoNetwork::BindNodes(int source_x, int source_y, int target_x, int target_y){
    QString string =  QString::number(source_x)+','+QString::number(source_y);
    dojoNode* source = NodeTable[string];

    string =  QString::number(target_x)+','+QString::number(target_y);
    dojoNode* target = NodeTable[string];

    //Calc length
    float x = abs(target_x-source_x);
    float y = abs(target_y - source_y);
    float length = sqrt(x*x + y*y);

    dojoSynapse* synapse = new dojoSynapse(source, target, length);

    source->AddTarget(synapse);
    target->AddSource(synapse);

    QString event;
    event += "4," + QString::number(x)+','+QString::number(y);

    emit dojoEvent(event);
}
