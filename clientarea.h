#ifndef CLIENTAREA_H
#define CLIENTAREA_H

#include <QColor>
#include <QImage>
#include <QPoint>
#include <QWidget>

#include "dojo/dojonetwork.h"

class ClientArea : public QWidget
{
    Q_OBJECT

public:
    ClientArea(QWidget *parent = 0, dojoNetwork* dojo = 0);

public :
    void BindNetwork();
    void ChangeYAccel (float newYAccel);
    void ChangeXAccel (float newXAccel);
    void ChangeViewDirection (float newDirection);

public slots:
    void ClientProcess();

signals :
    void AddToWatch(float* value, QString name, QColor color);

protected:
    void paintEvent(QPaintEvent *);
    void keyPressEvent(QKeyEvent *event);

private:
    void UpdateNetwork(quint8 mask);
    void resizeImage(QImage *image, const QSize &newSize);    

    dojoNetwork* dojoPtr;

    quint8 originalMask, bitMask;
    int shift;
    bool stop;
    int counter;

    float Sensor1, Sensor2, Sensor3,Sensor4,Sensor5,Sensor6, Sensor7, Sensor8;
    float Actuator1;

    QMultiHash<int, QString> Nodes;

};

#endif
