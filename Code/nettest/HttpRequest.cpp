#include "HttpRequest.h"

HttpRequest::HttpRequest(QNetworkReply *reply, QObject *parent) : QObject(parent),
    m_reply(reply),
    m_JSONResponse(0),
    m_StringResponse(0),
    m_DataResponse(0),
    m_Progress(0),
    m_ResponseType(JSON)
{
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(reciveError(QNetworkReply::NetworkError)));
    connect(reply, SIGNAL(uploadProgress(qint64 ,qint64)), this, SLOT( uploadProgress(qint64 ,qint64)));
    connect(reply, SIGNAL(downloadProgress(qint64 ,qint64)), this, SLOT( downloadProgress(qint64 ,qint64)));
    connect(reply, SIGNAL(finished()), this, SLOT( finished()));
}

HttpRequest *HttpRequest::responseString(HttpStringResponse response)
{
    m_StringResponse = response;
    return this;
}

HttpRequest *HttpRequest::responseJSON(HttpJSONResponse response)
{
    m_JSONResponse = response;
    return this;
}

HttpRequest *HttpRequest::responseData(HttpDataResponse response)
{
    m_DataResponse = response;
    return this;
}

HttpRequest *HttpRequest::progress(HttpProgress response)
{
    m_Progress = response;
    return this;
}

QNetworkRequest HttpRequest::creatNetworkRequest()
{
//    QJsonDocument jsonDocument = QJsonDocument::fromVariant(parameters);

//    QNetworkRequest request(url);
//    request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));
    return QNetworkRequest();
}

void HttpRequest::reciveError(QNetworkReply::NetworkError errorCode)
{
    qDebug() << "upLoadError  errorCode: " << (int)errorCode;
}

void HttpRequest::uploadProgress(qint64 bytesSent, qint64 bytesTotal)
{
    qDebug() << "bytesSent: " << bytesSent << "  bytesTotal: "<< bytesTotal;
}

void HttpRequest::downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    qDebug() << "bytesReceived: " << bytesReceived << "  bytesTotal: "<< bytesTotal;
}

void HttpRequest::finished()
{
    if(m_reply->error() == QNetworkReply::NoError){
        QByteArray bytes = m_reply->readAll();
        if(m_JSONResponse) {
            QJsonParseError error;
            QJsonDocument jsonDocument = QJsonDocument::fromJson(bytes,&error);
            m_JSONResponse(error.error == QJsonParseError::NoError,jsonDocument);
        }
        if(m_StringResponse){
            m_StringResponse(true,QString(bytes));
        }
        if(m_DataResponse){
            m_DataResponse(true,bytes);
        }
    } else {
        if(m_JSONResponse) {
            m_JSONResponse(false,QJsonDocument());
        }
        if(m_StringResponse){
            m_StringResponse(false,QString());
        }
        if(m_DataResponse){
            m_DataResponse(false,QByteArray());
        }
    }
}
