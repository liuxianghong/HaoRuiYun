#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <functional>
#include <QObject>
#include <QFile>
#include <QDir>
#include <QImage>

typedef std::function< void(const bool success, const QImage &image) > imageFileCompleteHandle;

class FileManager : public QObject
{
    Q_OBJECT

    explicit FileManager(QObject *parent = 0);
public:

    static FileManager *self();

    void getImage(QString url, imageFileCompleteHandle handle = nullptr);

    QString getLogPath();
private:
    bool saveImage(QImage *image, QString url);

    QImage getCacheImage(QString url);

private:
    QString m_cachePath;
signals:

public slots:
};

#endif // FILEMANAGER_H
