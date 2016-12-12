#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#include <QObject>
#include <QtNetwork>

typedef std::function< void(const QJsonDocument &document,const QNetworkReply::NetworkError &error) > HttpJSONResponse;
typedef std::function< void(const QString &string,const QNetworkReply::NetworkError &error) > HttpStringResponse;
typedef std::function< void(const QByteArray &data,const QNetworkReply::NetworkError &error) > HttpDataResponse;
typedef std::function< void(const qint64 bytedone, const qint64 bytetotal) > HttpProgress;

struct JQNetworkOnReceivedCallbackPackage
{
    std::function< void(const QJsonDocument &document) > succeedCallback = nullptr;
    std::function< void(const QNetworkReply::NetworkError &error) > failCallback = nullptr;
    std::function< void(const float &progress) > progressCallback = nullptr;
};

class HttpRequest : public QObject
{
    Q_OBJECT
public:
    explicit HttpRequest(QString url,const QVariant &variant = QVariant(),QObject *parent = 0);

    void responseString(HttpJSONResponse response);
    void responseJSON(HttpStringResponse response);
    void responseData(HttpDataResponse response);

    QNetworkRequest creatNetworkRequest();
signals:

public slots:
};

#endif // HTTPREQUEST_H
