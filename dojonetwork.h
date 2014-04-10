#ifndef DOJONETWORK_H
#define DOJONETWORK_H

#define TIME_SCALE 20

#include <QObject>
#include <QHash>

#include "dojonode.h"

class dojoNetwork : public QObject
{
    Q_OBJECT
public:
    explicit dojoNetwork(QObject *parent = 0);
    void CreateNode(int x, int y);
    void BindNodes(int source_x, int source_y, int target_x, int target_y);

    void CreateSensor(float* data, int x, int y);
    void CreateActuator(float* data, int x, int y);

    dojoNode* GetNodePtr(QString node);

signals:
    void dojoEvent(QString event);
public slots:
    void Process();

private :
    int counter;
    QHash <QString, dojoNode*> NodeTable;
};

#endif // DOJONETWORK_H
