#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include <QFile>

class Logger : public QObject
{
    Q_OBJECT

    explicit Logger(QObject *parent = 0);
public:
    static Logger *self();

    void setLogLeavl(int leavl);
    void logMsg(QByteArray msg);
signals:

public slots:

private:
    QString m_path;
    QFile m_file;
    int m_logLeavl;


    void creatFile();

    static void MessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg);
};

#endif // LOGGER_H
