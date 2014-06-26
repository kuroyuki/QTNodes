#include "graphwidget.h"
#include "node.h"
#include <math.h>
#include <QKeyEvent>

GraphWidget::GraphWidget(QWidget *parent, dojoNetwork* dojo)
    : QGraphicsView(parent)
{
    dojoPtr = dojo;
    scene = new QGraphicsScene(this);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);

    //set scene size
    scene->setSceneRect(0, 0, dojoPtr->GetNetworkSize().width()*50, dojoPtr->GetNetworkSize().height()*50);

    setScene(scene);
    setCacheMode(CacheBackground);
    setViewportUpdateMode(BoundingRectViewportUpdate);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(AnchorUnderMouse);
    scale(qreal(0.8), qreal(0.8));

    setWindowTitle(tr("Elastic Nodes"));

    addNode(1,-1);
    addNode(-1,1);
}

void GraphWidget::itemMoved()
{

}

void GraphWidget::keyPressEvent(QKeyEvent *event)
{

}

void GraphWidget::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);
}

#ifndef QT_NO_WHEELEVENT
void GraphWidget::wheelEvent(QWheelEvent *event)
{
    scaleView(pow((double)2, -event->delta() / 240.0));
}
#endif

void GraphWidget::drawBackground(QPainter *painter, const QRectF &rect)
{
    Q_UNUSED(rect);

    // Shadow
    QRectF sceneRect = this->sceneRect();
    QRectF rightShadow(sceneRect.right(), sceneRect.top() + 5, 5, sceneRect.height());
    QRectF bottomShadow(sceneRect.left() + 5, sceneRect.bottom(), sceneRect.width(), 5);
    if (rightShadow.intersects(rect) || rightShadow.contains(rect))
        painter->fillRect(rightShadow, Qt::darkGray);
    if (bottomShadow.intersects(rect) || bottomShadow.contains(rect))
        painter->fillRect(bottomShadow, Qt::darkGray);

    // Fill
    QLinearGradient gradient(sceneRect.topLeft(), sceneRect.bottomRight());
    gradient.setColorAt(0, Qt::white);
    gradient.setColorAt(1, Qt::lightGray);
    painter->fillRect(rect.intersected(sceneRect), gradient);
    painter->setBrush(Qt::NoBrush);
    painter->drawRect(sceneRect);
}

void GraphWidget::scaleView(qreal scaleFactor)
{
    qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    if (factor < 0.07 || factor > 100)
        return;

    scale(scaleFactor, scaleFactor);
}


void GraphWidget::zoomIn()
{
    scaleView(qreal(1.2));
}

void GraphWidget::zoomOut()
{
    scaleView(1 / qreal(1.2));
}
void GraphWidget::graphUpdate(QString event){
    //Parse event string
    QStringList list = event.split(",");

    int i = list.at(0).toInt();
    if(i==1)
       addNode(list.at(1).toInt(), list.at(2).toInt());
    else if(i==2){
        Node* source = getNode("1,-1");
        Node* target = getNode(list.at(2)+","+list.at(3));
        scene->addItem(new Edge(source, target));
    }
    else if(i==3){
        Node* target = getNode("-1,1");
        Node* source = getNode(list.at(1)+","+list.at(2));
        scene->addItem(new Edge(source, target));
    }
    else if(i ==4 ){
        Node* source = getNode(list.at(1)+","+list.at(2));
        Node* target = getNode(list.at(3)+","+list.at(4));
        scene->addItem(new Edge(source, target));
    }
    else if(i ==5 ){
        Node* node = getNode(list.at(1)+","+list.at(2));
        scene->removeItem(node);
    }
    else if(i ==6 ){
        //set new scene size
        scene->setSceneRect(0, 0, dojoPtr->GetNetworkSize().width()*50, dojoPtr->GetNetworkSize().height()*50);
    }
}
void GraphWidget::addNode(qreal x, qreal y){
    Node *node = new Node(this);
    node->setPos(50*x, 50*y);
    scene->addItem(node);
    QString string =  QString::number(x)+','+QString::number(y);
    Nodes[string] = node;
}
Node* GraphWidget::getNode(QString node){
    if(Nodes.contains(node))
        return Nodes[node];
    else return 0;
}
void GraphWidget::AddNodeToWatch(Node* node, QColor color){
    QString name = Nodes.key(node);
    float* data = dojoPtr->GetNodePtr(name)->GetVoltagePtr()    ;
    emit AddToWatch(data, name, color);
}
