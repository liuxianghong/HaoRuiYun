#include "KUploadFile.h"


KUploadFile *KUploadFile::d = 0;

KUploadFile::KUploadFile(QObject *parent)
    :QObject(parent)
{
//    QFileDialog dialog(this);
//    filename = dialog.getOpenFileName(0, "/");

    _uploadManager = new QNetworkAccessManager(this);
    connect(_uploadManager,SIGNAL(finished(QNetworkReply*)),SLOT(replyFinished(QNetworkReply*)));
    m_buf = NULL;
}

KUploadFile::~KUploadFile()
{
    if( m_buf != NULL ) delete[] m_buf;
}

KUploadFile * KUploadFile::getInstance()
{
    if (d == 0)
        d = new KUploadFile;
    return d;
}


void KUploadFile::uploadFile(QString uploadFilename)
{
    QFile file(uploadFilename);
    file.open(QIODevice::ReadOnly);
    int  file_len = file.size();

    QDataStream in(&file);
    m_buf = new char[file_len];
    in.readRawData( m_buf, file_len);
    file.close();

    QUrl url = QString("http://10.0.1.77:8080/TrainService/Uploadfile");
    QNetworkRequest request( url );

    request.setHeader(QNetworkRequest::ContentTypeHeader, "multipart/form-data");
    request.setRawHeader("filename", uploadFilename.section('/', -1, -1).toUtf8() );

    QByteArray arr = QByteArray( m_buf, file_len );
    _reply = _uploadManager->post( request , arr );

    connect(_reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(upLoadError(QNetworkReply::NetworkError)));
    connect(_reply, SIGNAL(uploadProgress ( qint64 ,qint64 )), this, SLOT( OnUploadProgress(qint64 ,qint64 )));

}


void KUploadFile::replyFinished(QNetworkReply *reply)
{
    if(reply->error() == QNetworkReply::NoError)
    {
        qDebug()<<"no error.....";
        QByteArray bytes = reply->readAll();  //获取字节
        QString result(bytes);  //转化为字符串
        qDebug()<<result;
    }
    else{
        qDebug() << "replyFinished:  " << reply->error() << " " <<reply->errorString();
    }

    QVariant status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    qDebug()<<status_code;
}

void KUploadFile::upLoadError(QNetworkReply::NetworkError errorCode)
{
    qDebug() << "upLoadError  errorCode: " << (int)errorCode;
}

void KUploadFile::OnUploadProgress(qint64 bytesSent, qint64 bytesTotal)
{
    qDebug() << "bytesSent: " << bytesSent << "  bytesTotal: "<< bytesTotal;
}
