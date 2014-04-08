#include <QtWidgets>

#include "clientarea.h"
#include "math.h"
#include <QTimer>

#define WIDE_RANGE 0.5

ClientArea::ClientArea(QWidget *parent, dojoNetwork* dojo)
    : QWidget(parent)
{
    setAttribute(Qt::WA_StaticContents);

    rectCenter.setX(250);
    rectCenter.setY(250);

    viewCenter.setX(250);
    viewCenter.setY(10);

    dojoPtr = dojo;

    originalMask = 7;
    shift = 0;
    bitMask = originalMask;

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(ClientProcess()));
    timer->start(50);

    grabKeyboard();
}
void ClientArea::InitializeNetwork(){

    dojoPtr->CreateSensor(&Sensor1, 1,1);
    dojoPtr->CreateSensor(&Sensor2, 2,1);
    dojoPtr->CreateSensor(&Sensor3, 3,1);
    dojoPtr->CreateSensor(&Sensor4, 4,1);
    dojoPtr->CreateSensor(&Sensor5, 5,1);
    dojoPtr->CreateSensor(&Sensor6, 6,1);

    dojoPtr->CreateActuator(&Actuator1, 4,4);

    dojoPtr->CreateNode(4,3);
    dojoPtr->CreateNode(2,2);
    dojoPtr->CreateNode(5,2);

    dojoPtr->BindNodes(1,1,2,2);
    dojoPtr->BindNodes(2,1,2,2);
    dojoPtr->BindNodes(3,1,2,2);

    dojoPtr->BindNodes(2,2,4,3);
    dojoPtr->BindNodes(5,2,4,3);
    dojoPtr->BindNodes(3,1,4,3);
    dojoPtr->BindNodes(4,1,4,3);

    dojoPtr->BindNodes(4,1,5,2);
    dojoPtr->BindNodes(5,1,5,2);
    dojoPtr->BindNodes(6,1,5,2);

    dojoPtr->BindNodes(2,2,4,3);
    dojoPtr->BindNodes(5,2,4,3);

    dojoPtr->BindNodes(4,3,4,4);

    UpdateNetwork();
}

void ClientArea::keyPressEvent(QKeyEvent *event){
    if(event->key() == Qt::Key_Left)
        shift--;
    else if (event->key() == Qt::Key_Right)
        shift++;

    if(shift>0)
        bitMask = originalMask>>shift;
    else
        bitMask = originalMask<<abs(shift);

    UpdateNetwork();
    update();
}
void ClientArea::UpdateNetwork(){
    Sensor1 = 0;
    Sensor2 = 0;
    Sensor3 = 100;
    Sensor4 = 100;
    Sensor5 = 0;
    Sensor6 = 0;
}

void ClientArea::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    int i;

    for (i=0; i<8; i++)
    {
        if(bitMask>>i & 0x01)
            painter.setBrush(Qt::white);
        else
            painter.setBrush(Qt::black);

        painter.drawRect(290-20*i,120,20,20);
    }
    quint8 color = Actuator1*255/100;
    painter.setBrush(QColor(color, color, color));
    painter.drawRect(280,280,20,20);

    //draw all synapses
    QList<QString> nodes = Nodes.values(4);
    for (int i = 0; i < nodes.size(); ++i) {
        QStringList list = nodes[i].split(",");
        int x1 = list.at(0).toInt();
        int y1 = list.at(1).toInt();
        int x2 = list.at(2).toInt();
        int y2 = list.at(3).toInt();

        painter.setBrush(QColor(0,0,0));
        painter.drawLine(30*x1+5,30*y1+5,30*x2+5,30*y2+5);
    }
    //draw all nodes
    nodes = Nodes.values(1);

    for (int i = 0; i < nodes.size(); ++i) {
        QStringList list = nodes[i].split(",");
        int x = list.at(0).toInt();
        int y = list.at(1).toInt();

        painter.setBrush(QColor(0,255,0));
        painter.drawRect(30*x,30*y,10,10);
    }
    //draw all sensors
    nodes = Nodes.values(2);

    for (int i = 0; i < nodes.size(); ++i) {
        QStringList list = nodes[i].split(",");
        int x = list.at(0).toInt();
        int y = list.at(1).toInt();

        painter.setBrush(QColor(255,0,0));
        painter.drawRect(30*x,30*y,10,10);
    }
    //draw all acts
    nodes = Nodes.values(3);
    for (int i = 0; i < nodes.size(); ++i) {
        QStringList list = nodes[i].split(",");
        int x = list.at(0).toInt();
        int y = list.at(1).toInt();

        painter.setBrush(QColor(0,0,255));
        painter.drawRect(30*x,30*y,10,10);
    }
}
void ClientArea::ClientProcess(){
    if(Actuator1 > 0)
        Actuator1 -= 1;

    update(78,78,25,25);
}
void ClientArea::ClientUpdate(QString event){
    //Parse event string
    QStringList list = event.split(",");
    switch (list.at(0).toInt()){
        //create node
        case 1 :
            Nodes.insert(1, list.at(1)+","+list.at(2));
            break;
        //create sensor
        case 2 :
            Nodes.insert(2, list.at(1)+","+list.at(2));
            break;
        //create act
        case 3 :
            Nodes.insert(3, list.at(1)+","+list.at(2));
            break;
        //bind nodes
        case 4 :
            Nodes.insert(4, list.at(1)+","+list.at(2)+","+list.at(3)+","+list.at(4));
            break;
        default :
            break;
    }
}
