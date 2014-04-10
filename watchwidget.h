#ifndef WATCHWIDGET_H
#define WATCHWIDGET_H

#include <QDockWidget>
#include "dojonetwork.h"
#include <QColor>
#include <QImage>
#include <QPoint>
#include <QWidget>
#include "C:/Users/Yuki/Documents/GitHub/qcustomplot/qcustomplot.h"

class WatchWidget : public QDockWidget
{
    Q_OBJECT
public:
    explicit WatchWidget(dojoNetwork* dojo = 0);

signals:

public slots:
    void AddPlot(QString node);
    void Process();

private :
    void setupRealtimeDataDemo(QCustomPlot *customPlot);

    dojoNetwork* dojoPtr;
    QTimer* dataTimer;
    QCustomPlot* Plot;
    dojoNode* Node;
};

#endif // WATCHWIDGET_H
