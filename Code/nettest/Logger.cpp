#include "Logger.h"
#include "FileManager.h"
#include <QDateTime>
#include <QtDebug>
//#include <stdio.h>
//#include <stdlib.h>
#include <QTimer>

static Logger *p_logger = 0;

Logger::Logger(QObject *parent) : QObject(parent)
  ,m_logLevel(LoggerOff)
  ,m_maxFileSize(10 * 1024 * 1024)
  ,m_maxFileCount(100)
  ,m_maxDiskSize(1024 * 1024 * 1024)
{
    QTimer::singleShot(0,[](){
        qInstallMessageHandler(Logger::MessageOutput);
    });
    m_path = FileManager::self()->getLogPath();
    QTimer::singleShot(0,this,SLOT(checkLog()));
}

Logger *Logger::self()
{
    if (p_logger == 0){
        p_logger = new Logger();
    }
    return p_logger;
}

void Logger::setLogLevel(LoggerLevel level)
{
    m_logLevel = level;
}

void Logger::logMsg(QByteArray msg)
{
    if (!m_file.isOpen()){
        creatFile();
    }
    m_file.write("\r\n");
    QString dateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss : ");
    m_file.write(dateTime.toUtf8());
    m_file.write(msg);
    m_file.flush();

    if (m_file.size() > m_maxFileSize){
        creatFile();
    }
}

void Logger::setMaxFileSize(qint64 size)
{
    m_maxFileSize = size;
}

void Logger::setMaxFileCount(int count)
{
    m_maxFileCount = count;
}

void Logger::setMaxDiskSize(qint64 size)
{
    m_maxDiskSize = size;
}

void Logger::creatFile(){
    if (m_file.isOpen()){
        m_file.flush();
        m_file.close();
    }
    QTimer::singleShot(0,this,SLOT(checkLog()));
    QString dateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd-HH-mm-ss");
    QDir dir(m_path);
    QString fileName = dir.absoluteFilePath(dateTime + ".log");
    m_file.setFileName(fileName);
    m_file.open(QIODevice::ReadWrite);
    m_file.write("/*****Logger Begin*****/");
    m_file.flush();
}

void Logger::checkLog(){
    QDir dir(m_path);
    QStringList filters;
    filters << "*.log";
    QFileInfoList list = dir.entryInfoList(filters,QDir::Files,QDir::Name);
    while (list.count() > m_maxFileCount && !list.isEmpty()) {
        QFileInfo var = list.first();
        dir.remove(var.fileName());
        list.removeFirst();
    }

    auto getTotalSize = [](const QFileInfoList &infoList,qint64 &totalSize) {
        totalSize = 0;
        foreach (QFileInfo var, infoList) {
            totalSize += var.size();
        }
    };
    qint64 size = 0;
    getTotalSize(list,size);

    while (size > m_maxDiskSize && !list.isEmpty()) {
        QFileInfo var = list.first();
        dir.remove(var.fileName());
        list.removeFirst();
        getTotalSize(list,size);
    }

}


void Logger::MessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    auto QtmessageTypeToLoggerLevel = [type](){
        switch (type)
        {
        case QtDebugMsg:
            return LoggerDebug;
        case QtInfoMsg:
            return LoggerInfo;
        case QtCriticalMsg:
            return LoggerCritical;
        case QtWarningMsg:
            return LoggerWarning;
        case QtFatalMsg:
            return LoggerCritical;
        default:
            return LoggerDebug;
        }
    };

    auto loggerLevelName = [type](){
        switch (type) {
        case QtDebugMsg:
            return "Debug: ";
        case QtInfoMsg:
            return "Info: ";
        case QtCriticalMsg:
            return "Critical: ";
        case QtWarningMsg:
            return "Warning: ";
        case QtFatalMsg:
            return "Fatal: ";
        default:
            return "Debug: ";
        }
    };

    LoggerLevel level = QtmessageTypeToLoggerLevel();
    LoggerLevel cLevel = Logger::self()->m_logLevel;
    if ((level & cLevel) == level){
        QString logString = loggerLevelName();
        logString.append(msg);
        if ((cLevel & LoggerVerbose) == LoggerVerbose){
            logString.append(" file: " + QString(context.file));
            logString.append(" function: " + QString(context.function));
            logString.append(" line: " + QString::number(context.line));
        }
        Logger::self()->logMsg(logString.toUtf8());
    }

    switch (type) {
    case QtDebugMsg:
    case QtInfoMsg:
        fprintf(stdout, msg.toLocal8Bit());
        fflush(stdout);
        break;
    case QtCriticalMsg:
    case QtWarningMsg:
        fprintf(stderr, msg.toLocal8Bit());
        fflush(stderr);
        break;
    case QtFatalMsg:
        fprintf(stderr, msg.toLocal8Bit());
        fflush(stderr);
        abort();
    default:
        break;
    }
}
