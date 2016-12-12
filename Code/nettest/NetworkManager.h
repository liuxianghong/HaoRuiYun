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
              const QMap<QString,QVariant>,
              std::function< void(const QJsonDocument &document) > succeed = nullptr,
              std::function< void(const QNetworkReply::NetworkError &error) >fail = nullptr,
              std::function< void(const float &progress) >progress = nullptr);

signals:

public slots:

private:
    QNetworkAccessManager *m_networkManager;
    QMap<QNetworkReply *,HttpRequest *> requsts;

private slots:

    void replyFinished(QNetworkReply *reply);

    void upLoadError(QNetworkReply::NetworkError errorCode);

    void OnUploadProgress(qint64 bytesSent, qint64 bytesTotal);
};

#endif // NETWORKMANAGER_H
