#include <QtWidgets>

#include "clientarea.h"
#include "math.h"
#include <QTimer>

#define WIDE_RANGE 0.5

ClientArea::ClientArea(QWidget *parent, dojoNetwork* dojo)
    : QWidget(parent)
{
    setAttribute(Qt::WA_StaticContents);
    setMinimumSize(200,50);
    dojoPtr = dojo;

    originalMask = 7;
    shift = 0;
    bitMask = originalMask;
    stop = false;
    counter = 0;

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(ClientProcess()));
    timer->start(10);

    grabKeyboard();
}
void ClientArea::BindNetwork(){

    dojoPtr->CreateSensor(&Sensor1, 1,1);

    dojoPtr->CreateActuator(&Actuator1,1,2);

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

    if(event->key() == Qt::Key_S){
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

    if(stop) value = 0;
    //fire each 100 ms (10 per second, 10 Hz)
    else if((counter == 0))// || (counter == 2))
        //voltage 60 volts
        value = 50;
    //cycle length
    if(counter>100) counter = 0;
    else counter++;

    Sensor1 = value *(mask & 0x01);
    Sensor2 = value *(mask>>1 & 0x01);
    Sensor3 = value *(mask>>2 & 0x01);
    Sensor4 = value *(mask>>3 & 0x01);
    Sensor5 = value *(mask>>4 & 0x01);
    Sensor6 = value *(mask>>5 & 0x01);

    if (Actuator1 > 255)
        Actuator1 = 255;
    if(Actuator1 > 0)
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

        painter.drawRect(160-20*i,20,20,20);
    }
    quint8 color = Actuator1;
    painter.setBrush(QColor(color, color, color));
    painter.drawRect(20,80,20,20);
}
void ClientArea::ClientProcess(){   
    UpdateNetwork(bitMask);
    update();
}

