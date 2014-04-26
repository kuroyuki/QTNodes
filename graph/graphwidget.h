#ifndef GRAPHWIDGET_H
#define GRAPHWIDGET_H

#include <QObject>
#include <QGraphicsView>
#include "node.h"

class GraphWidget : public QGraphicsView
{
    Q_OBJECT

public:
    GraphWidget(QWidget *parent = 0);

    void itemMoved();

public slots:
    void zoomIn();
    void zoomOut();
    void graphUpdate(QString event);


protected:
    void keyPressEvent(QKeyEvent *event);
    void timerEvent(QTimerEvent *event);
#ifndef QT_NO_WHEELEVENT
    void wheelEvent(QWheelEvent *event);
#endif
    void drawBackground(QPainter *painter, const QRectF &rect);

    void scaleView(qreal scaleFactor);
private:
    QGraphicsScene *scene;
    QHash<QString, Node*> Nodes;

    void addNode(qreal x, qreal y);
    Node* getNode(QString node);
};


#endif // GRAPHWIDGET_H
