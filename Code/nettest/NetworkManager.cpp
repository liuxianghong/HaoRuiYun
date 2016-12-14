#include "NetworkManager.h"

static NetworkManager *p_networkManager = 0;

NetworkManager::NetworkManager(QObject *parent) : QObject(parent)
{
    m_networkManager = new QNetworkAccessManager(this);
    connect(m_networkManager,SIGNAL(finished(QNetworkReply*)),SLOT(replyFinished(QNetworkReply*)));
    connect(m_networkManager
            ,SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>))
            ,SLOT(replySslErrors(QNetworkReply*,QList<QSslError>)));
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

HttpRequest *NetworkManager::get(const QString url, const QVariant parameters)
{
    QNetworkRequest request(url);
//    QSslConfiguration config ;
//     config.setPeerVerifyMode(QSslSocket::VerifyNone);
//     config.setProtocol(QSsl::SslV3);
//     request.setSslConfiguration(config);
    //request.setSslConfiguration(QSslConfiguration::defaultConfiguration());
    //request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));
    QNetworkReply *reply = m_networkManager->get(request);
    reply->ignoreSslErrors();
    HttpRequest *httpRequest = new HttpRequest(reply,0);
    return httpRequest;
}


void NetworkManager::replyFinished(QNetworkReply *reply)
{
    if(reply->error() == QNetworkReply::NoError)
    {

    }
    else{

    }
    QVariant status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    qDebug()<<status_code;
    HttpRequest *http = requsts[reply];
    requsts.remove(reply);
    http->deleteLater();
    reply->deleteLater();
}

void NetworkManager::replySslErrors(QNetworkReply *reply, QList<QSslError> errorList)
{
    qDebug()<<"replySslErrors"<<errorList;
    reply->ignoreSslErrors(errorList);
}

