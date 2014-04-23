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
    stop = false;
    counter = 0;

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(ClientProcess()));
    timer->start(20);

    grabKeyboard();
}
void ClientArea::InitializeNetwork(){

    dojoPtr->CreateNode(1,1);
    dojoPtr->CreateNode(2,1);
    dojoPtr->CreateSensor(&Sensor1, 1,1);
    dojoPtr->CreateSensor(&Sensor1, 2,1);

    dojoPtr->BindNodes(2,1,1,1);
    dojoPtr->CreateActuator(&Actuator1,1,1);

    UpdateNetwork(bitMask);
}

void ClientArea::keyPressEvent(QKeyEvent *event){
    if(event->key() == Qt::Key_Left)
        shift--;
    else if (event->key() == Qt::Key_Right)
        shift++;
    else if (event->key() == Qt::Key_Up)
        stop = true;
    else if (event->key() == Qt::Key_Down)
        stop = false;

    if(event->key() == Qt::Key_1){
        emit AddToWatch("1,1");
        emit SensToWatch(&Sensor1);
        emit ActToWatch(&Actuator1);
    }

    if(shift>0)
        bitMask = originalMask>>shift;
    else
        bitMask = originalMask<<abs(shift);

    UpdateNetwork(bitMask);
    update();
}
void ClientArea::UpdateNetwork(quint8 mask){
    quint8 value;
    counter++;

    if(stop) value = 0;
    else if((counter == 8)|| (counter == 9))//|| (counter == 12))
        value = 50;

    if(counter>12) counter = 0;

    Sensor1 = value *(mask & 0x01);
    Sensor2 = 0 *(mask>>1 & 0x01);
    Sensor3 = value *(mask>>2 & 0x01);
    Sensor4 = value *(mask>>3 & 0x01);
    Sensor5 = value *(mask>>4 & 0x01);
    Sensor6 = value *(mask>>5 & 0x01);

    if (Actuator1 > 255)
        Actuator1 = 255;
    else if(Actuator1 == 0)
        Actuator1 = 1;
    Actuator1--;
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
    quint8 color = Actuator1;
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
    UpdateNetwork(bitMask);
    update();
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
