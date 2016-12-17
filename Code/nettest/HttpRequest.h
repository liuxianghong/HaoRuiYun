#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#include <QObject>
#include <QtNetwork>

typedef std::function< void(const bool success, const QJsonDocument &document) > HttpJSONResponse;
typedef std::function< void(const bool success, const QString &string) > HttpStringResponse;
typedef std::function< void(const bool success, const QByteArray &data) > HttpDataResponse;
typedef std::function< void(const qint64 bytedone, const qint64 bytetotal) > HttpProgress;

class HttpRequest : public QObject
{
    Q_OBJECT

    explicit HttpRequest(QNetworkReply *reply,QObject *parent = 0);
public:
    HttpRequest *responseString(HttpStringResponse response);
    HttpRequest *responseJSON(HttpJSONResponse response);
    HttpRequest *responseData(HttpDataResponse response);
    HttpRequest *progress(HttpProgress response);

    QNetworkRequest creatNetworkRequest();
signals:

public slots:

private:
    QNetworkReply *m_reply;
    HttpJSONResponse m_JSONResponse;
    HttpStringResponse m_StringResponse;
    HttpDataResponse m_DataResponse;
    HttpProgress m_Progress;

    friend class HttpManager;

private slots:
    void reciveError(QNetworkReply::NetworkError errorCode);
    void uploadProgress(qint64 bytesSent, qint64 bytesTotal);
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void finished();
};

#endif // HTTPREQUEST_H
