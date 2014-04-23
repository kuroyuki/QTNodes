#include <QtWidgets>
#include "watchwidget.h"
#include <QTimer>

WatchWidget::WatchWidget(dojoNetwork* dojo) :
    QDockWidget()
{
    dojoPtr = dojo;
    setAllowedAreas(Qt::BottomDockWidgetArea);

    Plot = new QCustomPlot();
    setMinimumHeight(250);
    setupRealtimeDataDemo(Plot);
    setWidget(Plot);

    // setup a timer that repeatedly calls MainWindow::realtimeDataSlot:
    Timer = new QTimer(this);
    connect(Timer, SIGNAL(timeout()), this, SLOT(Process()));
    // setup a timer that repeatedly calls MainWindow::realtimeDataSlot:
    Timer->start(0);
}
void WatchWidget::AddPlot(QString synapse){

    QStringList list = synapse.split(",");
    dojoNode* source = dojoPtr->GetNodePtr(list.at(0)+","+list.at(1));

    quint8 a = (quint8) qrand();
    AddGraph(source->GetVoltagePtr(), synapse, QColor(0,a,a));
}
void WatchWidget::AddSensor(float* value){
    AddGraph(value, "sensor", Qt::green);
}
void WatchWidget::AddAct(float* value){
    AddGraph(value, "act", Qt::red);
}
void WatchWidget::AddGraph(float* value, QString name, QColor color){
    if(!graphTable[value]){

        QCPGraph* graph =  Plot->addGraph(); // blue line
        graph->setBrush(Qt::NoBrush);
        graph->setPen(QPen(color));
        graph->setName(name);

        graphTable[value] = graph;
    }
}

void WatchWidget::setupRealtimeDataDemo(QCustomPlot *Plot)
{
  Plot->setNotAntialiasedElements(QCP::aeAll);
  QFont font;
  font.setStyleStrategy(QFont::NoAntialias);
  Plot->xAxis->setTickLabelFont(font);
  Plot->yAxis->setTickLabelFont(font);
  Plot->legend->setFont(font);

  Plot->xAxis->setTickLabelType(QCPAxis::ltDateTime);
  Plot->xAxis->setDateTimeFormat("hh:mm:ss");
  Plot->xAxis->setAutoTickStep(false);
  Plot->xAxis->setTickStep(2);
  Plot->axisRect()->setupFullAxesBox();

  // make left and bottom axes transfer their ranges to right and top axes:
  connect(Plot->xAxis, SIGNAL(rangeChanged(QCPRange)), Plot->xAxis2, SLOT(setRange(QCPRange)));
  connect(Plot->yAxis, SIGNAL(rangeChanged(QCPRange)), Plot->yAxis2, SLOT(setRange(QCPRange)));
}
void WatchWidget::Process()
{
  double key = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;

  static double lastPointKey = 0;
  if (key-lastPointKey > 0.01) // at most add point every 10 ms
  {
    QHashIterator<float*, QCPGraph*> i(graphTable);
    while (i.hasNext()) {
      i.next();
      double a;
      a = (double) *i.key();
      i.value()->addData(key, a);
      i.value()->removeDataBefore(key-8);
      i.value()->rescaleValueAxis(true);
    }
    lastPointKey = key;
  }
  // make key axis range scroll with the data (at a constant range size of 8):
  Plot->xAxis->setRange(key+0.25, 8, Qt::AlignRight);
  Plot->replot();

}
