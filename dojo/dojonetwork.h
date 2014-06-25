#ifndef DOJONETWORK_H
#define DOJONETWORK_H

#define TIME_SCALE 10

#include <QObject>
#include <QHash>
#include <QTimer>
#include <QFile>
#include <QTextStream>

#include "dojonode.h"

class dojoNetwork : public QObject
{
    Q_OBJECT
public:
    explicit dojoNetwork(QObject *parent = 0);
    void LoadNetwork(QString name);
    void CreateNode(int x, int y);
    void DeleteNode(int x, int y);
    void BindNodes(int source_x, int source_y, int target_x, int target_y);

    void CreateSensor(float* data, int x, int y);
    void CreateActuator(float* data, int x, int y);

    dojoNode* GetNodePtr(QString node);

signals:
    void dojoEvent(QString event);
public slots:
    void Process();

private :
    QHash <QString, dojoNode*> NodeTable;
    QTimer *timer;
    QString networkName;

    void ParseConfigFile(QFile* file);

};

#endif // DOJONETWORK_H
