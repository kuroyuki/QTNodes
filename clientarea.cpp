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

    dojoPtr->CreateActuator(&Actuator1, 4,2);

    dojoPtr->BindNodes(1,1,4,2);
    dojoPtr->BindNodes(2,1,4,2);
    dojoPtr->BindNodes(3,1,4,2);
    dojoPtr->BindNodes(4,1,4,2);
    dojoPtr->BindNodes(5,1,4,2);
    dojoPtr->BindNodes(6,1,4,2);

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

        painter.drawRect(190-20*i,20,20,20);
    }
    quint8 color = Actuator1*255/100;
    painter.setBrush(QColor(color, color, color));
    painter.drawRect(80,80,20,20);


}
void ClientArea::ClientProcess(){
    if(Actuator1 > 0)
        Actuator1 -= 1;

    update(78,78,25,25);
}
void ClientArea::ClientUpdate(QString text){
    //Parse event string

}
