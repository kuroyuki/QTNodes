#include <QtWidgets>
#include "watchwidget.h"
#include <QTimer>

WatchWidget::WatchWidget() :
    QDockWidget()
{
    setWindowTitle("Watch");
    setAllowedAreas(Qt::BottomDockWidgetArea);

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(new QLabel(tr("Watching nodes:")), 1,1);

    watchingList =  new QComboBox();
    watchingList->setInsertPolicy(QComboBox::InsertAtCurrent);
    layout->addWidget(watchingList, 1,2);

    Plot = new QCustomPlot();

    setMinimumHeight(250);
    setupRealtimeDataDemo(Plot);
    layout->addWidget(Plot, 2,1,2,8);

    layout->setSizeConstraint(QLayout::SetMinimumSize);

    QWidget* child = new QWidget();
    child->setLayout(layout);
    setWidget(child);

    // setup a timer that repeatedly update plots
    Timer = new QTimer(this);
    connect(Timer, SIGNAL(timeout()), this, SLOT(Process()));

    //need to stop plot updates if widget is not visible
    connect(this, SIGNAL(visibilityChanged(bool)),this, SLOT(VisibilityChanged(bool)));

    // start update plots as quick as we can
    Timer->start(1);
}
void WatchWidget::AddGraph(float* value, QString name, QColor color){
    if(!graphTable[value]){

        QCPGraph* graph =  Plot->addGraph(); // blue line
        graph->setBrush(Qt::NoBrush);
        graph->setPen(QPen(color));
        graph->setName(name);

        graphTable[value] = graph;

        watchingList->addItem(name);

        emit AddToTerminal(name+" added to Watch list");
    }
    else{
        graphTable.remove(value);
        watchingList->removeItem(watchingList->findText(name));
        emit AddToTerminal(name+" removed from Watch list");
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
void WatchWidget::VisibilityChanged(bool isVisible){
    if(isVisible == true)
        Timer->start(1);
    else
        Timer->stop();
}
