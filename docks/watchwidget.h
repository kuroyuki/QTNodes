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
    explicit WatchWidget();

signals:
    void AddToTerminal(QString string);

public slots:
    void AddGraph(float* value, QString name, QColor color);
    void VisibilityChanged(bool isVisible);
    void Process();

private :
    void setupRealtimeDataDemo(QCustomPlot *customPlot);

    QCustomPlot* Plot;
    QComboBox* watchingList;

    QHash <float*, QCPGraph*> graphTable;

    QTimer *Timer;
};

#endif // WATCHWIDGET_H
