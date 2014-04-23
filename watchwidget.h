#ifndef WATCHWIDGET_H
#define WATCHWIDGET_H

#include <QDockWidget>
#include "dojo/dojonetwork.h"
#include <QColor>
#include <QImage>
#include <QPoint>
#include <QWidget>
#include "../qcustomplot/qcustomplot.h"

class WatchWidget : public QDockWidget
{
    Q_OBJECT
public:
    explicit WatchWidget(dojoNetwork* dojo = 0);

signals:

public slots:
    void AddPlot(QString node);
    void AddSensor(float* value);
    void AddAct(float* value);
    void Process();

private :
    void setupRealtimeDataDemo(QCustomPlot *customPlot);

    dojoNetwork* dojoPtr;
    QTimer* dataTimer;
    QCustomPlot* Plot;
    dojoNode* Node;

    float* Sensor;
    float* Act;

    QTimer *Timer;
};

#endif // WATCHWIDGET_H
