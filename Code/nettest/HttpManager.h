#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <functional>

#include <QObject>
#include <QtNetwork>
#include "HttpRequest.h"


class HttpManager : public QObject
{
    Q_OBJECT

    explicit HttpManager(QObject *parent = 0);
public:
    static HttpManager *self();

    HttpRequest *post(const QString url,
                      const QVariant parameters,
                      const QVariantMap headers = QVariantMap());

    HttpRequest *get(const QString url,
                     const QVariant parameters = QVariant(),
                     const QVariantMap headers = QVariantMap());

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
