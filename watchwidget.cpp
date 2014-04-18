#include <QtWidgets>
#include "watchwidget.h"
#include <QTimer>

WatchWidget::WatchWidget(dojoNetwork* dojo) :
    QDockWidget()
{
    dojoPtr = dojo;
    setAllowedAreas(Qt::BottomDockWidgetArea);

    Sensor = 0;
    Act = 0;

    Plot = new QCustomPlot();
    setMinimumHeight(250);
    setupRealtimeDataDemo(Plot);
    setWidget(Plot);

    // setup a timer that repeatedly calls MainWindow::realtimeDataSlot:
    Timer = new QTimer(this);
    connect(Timer, SIGNAL(timeout()), this, SLOT(Process()));
}
void WatchWidget::AddPlot(QString synapse){

    QStringList list = synapse.split(",");
    dojoNode* source = dojoPtr->GetNodePtr(list.at(0)+","+list.at(1));
    dojoNode* target = dojoPtr->GetNodePtr(list.at(2)+","+list.at(3));
    Synapse = source->GetSynapse(target);

    // setup a timer that repeatedly calls MainWindow::realtimeDataSlot:
     Timer->start(0);

}
void WatchWidget::AddSensor(float* value){
    Sensor = value;
    Timer->start(0);
}
void WatchWidget::AddAct(float* value){
    Act = value;
    Timer->start(0);
}
void WatchWidget::setupRealtimeDataDemo(QCustomPlot *Plot)
{
  Plot->setNotAntialiasedElements(QCP::aeAll);
  QFont font;
  font.setStyleStrategy(QFont::NoAntialias);
  Plot->xAxis->setTickLabelFont(font);
  Plot->yAxis->setTickLabelFont(font);
  Plot->legend->setFont(font);

  Plot->addGraph(); // blue line
  Plot->graph(0)->setPen(QPen(Qt::blue));
  Plot->graph(0)->setBrush(QBrush(QColor(240, 255, 200)));
  Plot->graph(0)->setAntialiasedFill(false);
  Plot->addGraph(); // red line
  Plot->graph(1)->setPen(QPen(Qt::red));
  Plot->graph(0)->setChannelFillGraph(Plot->graph(1));

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

    // add data to lines:
    Plot->graph(0)->addData(key, *Sensor);
    Plot->graph(1)->addData(key, *Act);

    // remove data of lines that's outside visible range:
    Plot->graph(0)->removeDataBefore(key-8);
    Plot->graph(1)->removeDataBefore(key-8);

    // rescale value (vertical) axis to fit the current data:
    Plot->graph(0)->rescaleValueAxis();
    Plot->graph(1)->rescaleValueAxis(true);
    lastPointKey = key;
  }
  // make key axis range scroll with the data (at a constant range size of 8):
  Plot->xAxis->setRange(key+0.25, 8, Qt::AlignRight);
  Plot->replot();

}
