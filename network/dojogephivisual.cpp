#include "dojogephivisual.h"
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>

dojoGephiVisual::dojoGephiVisual(QObject *parent) :
    QObject(parent)
{

}
void dojoGephiVisual::sendHttpPost(){
    QNetworkRequest request;
    request.setUrl(QUrl("http://localhost:8080/workspace0?operation=updateGraph"));
    request.setRawHeader("User-Agent", "MyOwnBrowser 1.0");

    QNetworkReply *reply = manager->get(request);
    connect(reply, SIGNAL(readyRead()), this, SLOT(slotReadyRead()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),
            this, SLOT(slotError(QNetworkReply::NetworkError)));
    connect(reply, SIGNAL(sslErrors(QList<QSslError>)),
            this, SLOT(slotSslErrors(QList<QSslError>)));
}
