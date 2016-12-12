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
                          , const QVariant parameters
                          )
{
    QJsonDocument jsonDocument = QJsonDocument::fromVariant(parameters);

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));
    QNetworkReply *reply = m_networkManager->post(request,jsonDocument.toJson());
    HttpRequest *httpRequest = new HttpRequest(reply,0);
    requsts[reply] = httpRequest;
    return httpRequest;
}


void NetworkManager::replyFinished(QNetworkReply *reply)
{
    if(reply->error() == QNetworkReply::NoError)
    {
        qDebug()<<"no error.....";
//        QByteArray bytes = reply->readAll();  //获取字节
//        QJsonParseError error;
//        QJsonDocument jsonDocument = QJsonDocument::fromJson(bytes,&error);
//        if (error.error == QJsonParseError::NoError) {
//            if (requsts.contains(reply)){
//                JQNetworkOnReceivedCallbackPackage package = requsts[reply];
//                if (package.succeedCallback) {
//                    package.succeedCallback(jsonDocument);
//                }
//            }
//        }
//        QString result(bytes);  //转化为字符串
//        qDebug()<<result;

    }
    else{
        qDebug() << "replyFinished:  " << reply->error() << " " <<reply->errorString();
//        if (requsts.contains(reply)){
//            JQNetworkOnReceivedCallbackPackage package = requsts[reply];
//            if (package.failCallback) {
//                package.failCallback(reply->error());
//            }
//        }
    }

    QVariant status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    qDebug()<<status_code;
    HttpRequest *http = requsts[reply];
    requsts.remove(reply);
    http->deleteLater();
    reply->deleteLater();
}

