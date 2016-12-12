#include "HttpRequest.h"

HttpRequest::HttpRequest(QString url, const QVariant &variant, QObject *parent) : QObject(parent)
{
    variant.isValid()
}

void HttpRequest::responseString(HttpJSONResponse response)
{

}

void HttpRequest::responseJSON(HttpStringResponse response)
{

}

void HttpRequest::responseData(HttpDataResponse response)
{

}

QNetworkRequest HttpRequest::creatNetworkRequest()
{
    QJsonDocument jsonDocument = QJsonDocument::fromVariant(parameters);

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));
}
