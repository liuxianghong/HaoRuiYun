#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include <QFile>

enum LoggerLevel{

    /**
     *  No logs
     */
    LoggerOff = 0,

    /**
     *  Error logs only
     */
    LoggerCritical = (1 << 1),

    /**
     *  Error and warning logs
     */
    LoggerWarning = (1 << 2 | LoggerCritical),

    /**
     *  Error, warning and info logs
     */
    LoggerInfo = (1 << 3 | LoggerWarning),

    /**
     *  Error, warning, info and debug logs
     */
    LoggerDebug = (1 << 4 | LoggerInfo),

    /**
     *  Error, warning, info, debug and verbose logs
     */
    LoggerVerbose = (1 << 5 | LoggerDebug),

    /**
     *  All logs (1...11111)
     */
    LoggerAll = 0xFFFF
};

class Logger : public QObject
{
    Q_OBJECT

    explicit Logger(QObject *parent = 0);
public:
    static Logger *self();

    void setLogLevel(LoggerLevel level);
    void logMsg(QByteArray msg);
    void setMaxFileSize(qint64 size);
    void setMaxFileCount(int count);
    void setMaxDiskSize(qint64 size);
signals:

public slots:

private:
    QString m_path;
    QFile m_file;
    LoggerLevel m_logLevel;
    qint64 m_maxFileSize;
    int m_maxFileCount;
    qint64 m_maxDiskSize;


    void creatFile();

    static void MessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg);
private slots:
    void checkLog();
};

#endif // LOGGER_H
