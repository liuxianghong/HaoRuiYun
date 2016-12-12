#include "NetworkManager.h"

static NetworkManager *p_networkManager = 0;

NetworkManager::NetworkManager(QObject *parent) : QObject(parent)
{
    m_networkManager = new QNetworkAccessManager(this);
    connect(m_networkManager,SIGNAL(finished(QNetworkReply*)),SLOT(replyFinished(QNetworkReply*)));
}

NetworkManager *NetworkManager::self()
{
    if (p_networkManager == 0){
        p_networkManager = new NetworkManager(0);
    }
    return p_networkManager;
}

HttpRequest *NetworkManager::post(const QString url
                          , const QMap<QString, QVariant> parameters
                          , std::function<void (const QJsonDocument &)> succeed
                          , std::function<void (const QNetworkReply::NetworkError &)> fail
                          , std::function<void (const float &)> progress)
{
    QJsonDocument jsonDocument = QJsonDocument::fromVariant(parameters);

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));
    QNetworkReply *_reply = m_networkManager->post(request,jsonDocument.toJson());

    JQNetworkOnReceivedCallbackPackage package;
    package.succeedCallback = succeed;
    package.failCallback = fail;
    package.progressCallback = progress;
    requsts[_reply] = package;
    qDebug() << _reply;
    connect(_reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(upLoadError(QNetworkReply::NetworkError)));
    connect(_reply, SIGNAL(uploadProgress ( qint64 ,qint64 )), this, SLOT( OnUploadProgress(qint64 ,qint64 )));

}


void NetworkManager::replyFinished(QNetworkReply *reply)
{
    if(reply->error() == QNetworkReply::NoError)
    {
        qDebug()<<"no error.....";
        QByteArray bytes = reply->readAll();  //获取字节
        QJsonParseError error;
        QJsonDocument jsonDocument = QJsonDocument::fromJson(bytes,&error);
        if (error.error == QJsonParseError::NoError) {
            if (requsts.contains(reply)){
                JQNetworkOnReceivedCallbackPackage package = requsts[reply];
                if (package.succeedCallback) {
                    package.succeedCallback(jsonDocument);
                }
            }
        }
        QString result(bytes);  //转化为字符串
        qDebug()<<result;

    }
    else{
        qDebug() << "replyFinished:  " << reply->error() << " " <<reply->errorString();
        if (requsts.contains(reply)){
            JQNetworkOnReceivedCallbackPackage package = requsts[reply];
            if (package.failCallback) {
                package.failCallback(reply->error());
            }
        }
    }

    QVariant status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    qDebug()<<status_code;
    requsts.remove(reply);
}

void NetworkManager::upLoadError(QNetworkReply::NetworkError errorCode)
{
    qDebug() << sender();
    qDebug() << "upLoadError  errorCode: " << (int)errorCode;
}

void NetworkManager::OnUploadProgress(qint64 bytesSent, qint64 bytesTotal)
{
    qDebug() << sender();
    qDebug() << "bytesSent: " << bytesSent << "  bytesTotal: "<< bytesTotal;
}
