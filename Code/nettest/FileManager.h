#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QObject>
#include <QFile>
#include <QDir>
#include <QImage>

class FileManager : public QObject
{
    Q_OBJECT

    explicit FileManager(QObject *parent = 0);
public:

    static FileManager *self();

    bool saveImage(QImage *image, QString url);

    QImage getImage(QString url);

private:
    QString m_cachePath;
signals:

public slots:
};

#endif // FILEMANAGER_H
