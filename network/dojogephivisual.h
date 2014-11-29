#ifndef DOJOGEPHIVISUAL_H
#define DOJOGEPHIVISUAL_H

#include <QObject>

class dojoGephiVisual : public QObject
{
    Q_OBJECT
public:
    explicit dojoGephiVisual(QObject *parent = 0);

signals:

public slots:
    void dojoGephiVisual::sendHttpPost();

};

#endif // DOJOGEPHIVISUAL_H
