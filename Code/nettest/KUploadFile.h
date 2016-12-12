#ifndef KUPLOADFILE_H
#define KUPLOADFILE_H

#include <QObject>
#include <QtNetwork>

class KUploadFile : public QObject
{
    Q_OBJECT
public:
    explicit KUploadFile(QObject *parent = 0);

    ~KUploadFile();
    void uploadFile(QString uploadFilename);
    static KUploadFile * getInstance();
signals:

public slots:

private:
    QNetworkAccessManager *_uploadManager;
    QNetworkReply *_reply;
    char *m_buf;
    QString filename;
    static KUploadFile *d;

private slots:

    void replyFinished(QNetworkReply *reply);

    void upLoadError(QNetworkReply::NetworkError errorCode);

    void OnUploadProgress(qint64 bytesSent, qint64 bytesTotal);
};

#endif // KUPLOADFILE_H
