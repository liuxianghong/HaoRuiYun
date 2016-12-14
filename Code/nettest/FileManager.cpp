#include "FileManager.h"
#include <QCoreApplication>
#include <QDebug>
#include <QCryptographicHash>
#include "NetworkManager.h"
#include <QPixmap>

static FileManager *p_fileManager = 0;

FileManager *FileManager::self()
{
    if (p_fileManager == 0){
        p_fileManager = new FileManager(0);
    }
    return p_fileManager;
}

void FileManager::getImage(QString url, imageFileCompleteHandle handle)
{
    QImage image = getCacheImage(url);
    if (!image.isNull()){
        if (handle)
            handle(true, image);
    }

    NetworkManager::self()->get(url)->responseData([=](const bool success, const QByteArray &data){
        QImage image;
        if (success){
            image.loadFromData(data);
            saveImage(&image,url);
        }
        if (handle)
            handle(success, image);
    });
}

bool FileManager::saveImage(QImage *image, QString url)
{
    if (image->isNull())
        return false;
    QDir dir = QDir(m_cachePath);
    dir.mkdir("Images");
    if (dir.cd("Images")){
        QString urlMD5 = QCryptographicHash::hash(url.toUtf8(), QCryptographicHash::Md5).toHex();
        QString fileName = dir.absoluteFilePath(urlMD5);
        qDebug()<<fileName;
        return image->save(fileName,"PNG");
    }
    return false;
}

QImage FileManager::getCacheImage(QString url)
{
    QDir dir = QDir(m_cachePath);
    dir.cd("Images");
    QString urlMD5 = QCryptographicHash::hash(url.toUtf8(), QCryptographicHash::Md5).toHex();
    QString fileName = dir.absoluteFilePath(urlMD5);
    return QImage(fileName,"PNG");
}

FileManager::FileManager(QObject *parent) : QObject(parent)
{
    QDir dir = QDir(QCoreApplication::applicationDirPath()+ "/cache");
    m_cachePath = dir.path();
    if (!dir.exists()){
        dir.cdUp();
        dir.mkdir("cache");
    }
    qDebug()<<m_cachePath;
}
