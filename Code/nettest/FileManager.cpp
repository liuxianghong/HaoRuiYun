#include "FileManager.h"
#include <QCoreApplication>
#include <QDebug>

static FileManager *p_fileManager = 0;

FileManager *FileManager::self()
{
    if (p_fileManager == 0){
        p_fileManager = new FileManager(0);
    }
    return p_fileManager;
}

bool FileManager::saveImage(QImage *image, QString url)
{
    QDir dir = QDir(m_cachePath);
    dir.mkdir("Images");
    if (dir.cd("Images")){
        QString fileName = dir.absoluteFilePath(url.toUtf8().toHex());
        qDebug()<<fileName;
        return image->save(fileName,"PNG");
    }
    return false;
}

QImage FileManager::getImage(QString url)
{
    QDir dir = QDir(m_cachePath);
    dir.cd("Images");
    QString fileName = dir.absoluteFilePath(url.toUtf8().toHex());
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
