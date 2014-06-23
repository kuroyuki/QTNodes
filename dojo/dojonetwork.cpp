#include "dojonetwork.h"
#include <QTimer>
#include <QStringList>

#include "math.h"

dojoNetwork::dojoNetwork(QObject *parent) :
    QObject(parent)
{
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(Process()));
    timer->start(TIME_SCALE);
}
void dojoNetwork::LoadNetwork(QString name){

    networkName = name+".dojo";

    QFile configFile(networkName);
    if (!configFile.open(QIODevice::ReadWrite | QIODevice::Text))
       return;

    while (!configFile.atEnd()) {
       QString line = configFile.readLine();
       QStringList list = line.split(",");
       int source_x = list.at(0).toInt();
       int source_y = list.at(1).toInt();
       int target_x = list.at(2).toInt();
       int target_y = list.at(3).toInt();

       //check source node
       if(!NodeTable.contains(list.at(0).toInt()+","+ list.at(1).toInt()))
           CreateNode(source_x,source_y);
       //check target node
       if(!NodeTable.contains(list.at(2).toInt()+","+ list.at(2).toInt()))
           CreateNode(target_x,target_y);
       //bind nodes
       BindNodes(source_x,source_y,target_x,target_y);
    }
    configFile.close();
    QString event;
    event = "network loaded from " + networkName;
    emit dojoEvent(event);

}

void dojoNetwork::Process(){
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
    if(!NodeTable.contains(string))
        NodeTable[string] = new dojoNode(1);

    QString event;
    event += "1," + QString::number(x)+','+QString::number(y);

    emit dojoEvent(event);
}
void dojoNetwork::CreateSensor(float* data, int x, int y){
    QString string =  QString::number(x)+','+QString::number(y);

    if(!NodeTable.contains(string))
        CreateNode(x,y);

    dojoNode* target = NodeTable[string];

    dojoSensorSynapse* synapse = new dojoSensorSynapse(data, target);

    target->AddSource(synapse);

    QString event;
    event += "2," +QString::number((int)data)+','+ QString::number(x)+','+QString::number(y);
    emit dojoEvent(event);
}

void dojoNetwork::CreateActuator(float* data, int x, int y){
    QString string =  QString::number(x)+','+QString::number(y);

    if(!NodeTable.contains(string))
        CreateNode(x,y);

    dojoNode* source = NodeTable[string];

    dojoMotorSynapse* synapse = new dojoMotorSynapse(source, data, 255);

    source->AddTarget(synapse);

    QString event;
    event += "3," + QString::number(x)+','+QString::number(y)+','+QString::number((int)data);

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

    dojoSynapse* synapse = new dojoChemicalSynapse(source, target, length);

    source->AddTarget(synapse);
    target->AddSource(synapse);

    /*
    QFile file(networkName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&file);
    out  <<  QString::number(source_x)<<','<<QString::number(source_y)<<','<<QString::number(target_x)<<','<<QString::number(target_y);
    file.close();
*/
    QString event;
    event += "4," + QString::number(source_x)+','+QString::number(source_y)+','+ QString::number(target_x)+','+QString::number(target_y);

    emit dojoEvent(event);
}
dojoNode* dojoNetwork::GetNodePtr(QString node){
    dojoNode* nodePtr = this->NodeTable[node];
    return nodePtr;
}
