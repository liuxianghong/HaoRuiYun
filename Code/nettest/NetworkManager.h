#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <functional>

#include <QObject>
#include <QtNetwork>
#include "HttpRequest.h"


class NetworkManager : public QObject
{
    Q_OBJECT

    explicit NetworkManager(QObject *parent = 0);
public:
    static NetworkManager *self();

    HttpRequest *post(const QString url,
              const QVariant parameters);

    HttpRequest *get(const QString url,
              const QVariant parameters = QVariant());

signals:

public slots:

private:
    QNetworkAccessManager *m_networkManager;
    QMap<QNetworkReply *,HttpRequest *> requsts;

private slots:

    void replyFinished(QNetworkReply *reply);
    void replySslErrors(QNetworkReply *reply,QList<QSslError> errorList);
};

#endif // NETWORKMANAGER_H
