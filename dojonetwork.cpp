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
/*
 *
    an: Add node
    cn: Change node
    dn: Delete node
    as: Add synapse
    cs: Change synapse
    ds: Delete synapse
The events are currently represented in the JSON format as follows:

{<event_type>:{<object_identifier>:{<attribute_name>:<attribute_value>,<attribute_name>:<attribute_value>}}}

Following, we show a list of events with some examples for each type of event, represented in the current JSON format implementation:

{"an":{"A":{"label":"Streaming Node A","size":2}}} // add node A
{"an":{"B":{"label":"Streaming Node B","size":1}}} // add node B
{"an":{"C":{"label":"Streaming Node C","size":1}}} // add node C
{"ae":{"AB":{"source":"A","target":"B","directed":false,"weight":2}}} // add edge A->B
{"ae":{"BC":{"source":"B","target":"C","directed":false,"weight":1}}} // add edge B->C
{"ae":{"CA":{"source":"C","target":"A","directed":false,"weight":2}}} // add edge C->A
{"cn":{"C":{"size":2}}}  // changes the size attribute to 2
{"cn":{"B":{"label":null}}}  // removes the label attribute
{"ce":{"AB":{"label":"From A to B"}}} // add the label attribute
{"de":{"BC":{}}} // delete edge BC
{"de":{"CA":{}}} // delete edge CA
{"dn":{"C":{}}}  // delete node C

With this format it is possible to put more than one object in each event, as in the following example:

{"an":{
    "A":{"label":"Streaming Node A","size":2}
    "B":{"label":"Streaming Node B","size":1}
    "C":{"label":"Streaming Node C","size":1}
  }
}
 *
 **/
void dojoNetwork::CreateNode(int x, int y){
    QString string =  QString::number(x)+','+QString::number(y);
    NodeTable[string] = new dojoNode();

    QJsonObject json;
    json.insert("create", QString::number(x)+","+QString::number(y));

    emit dojoEvent(json);
}
void dojoNetwork::CreateSensor(float* data, int x, int y){
    QString string =  QString::number(x)+','+QString::number(y);
    NodeTable[string] = new dojoSensor(data);

    QJsonObject json;
    json.insert("create sensor", QString::number(x)+","+QString::number(y));

    emit dojoEvent(json);
}

void dojoNetwork::CreateActuator(float* data, int x, int y){
    QString string =  QString::number(x)+','+QString::number(y);
    NodeTable[string] = new dojoActuator(data);

    QJsonObject json;
    json.insert("create act", QString::number(x)+","+QString::number(y));

    emit dojoEvent(json);
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

    QJsonObject json;
    json.insert("bind", QString::number(x)+","+QString::number(y));
    json.insert("source", QString::number(source_x)+","+QString::number(source_y));
    json.insert("target", QString::number(target_x)+","+QString::number(target_y));

    emit dojoEvent(json);
}
