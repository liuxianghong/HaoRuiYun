#include "Logger.h"
#include "FileManager.h"
#include <QDateTime>
#include <QtDebug>
#include <stdio.h>
#include <stdlib.h>

static Logger *p_logger = 0;

Logger::Logger(QObject *parent) : QObject(parent)
  ,m_logLeavl(0)
{
    m_path = FileManager::self()->getLogPath();
    QDir dir(m_path);
    QStringList filters;
    filters << "*.log";
    QFileInfoList list = dir.entryInfoList(filters,QDir::Files | QDir::Writable,QDir::Name);
    if (!list.isEmpty()){
        QFileInfo info = list.first();
        info.created();
        info.size();
    }
    creatFile();

    qInstallMessageHandler(Logger::MessageOutput);
}

Logger *Logger::self()
{
    if (p_logger == 0){
        p_logger = new Logger();
    }
    return p_logger;
}

void Logger::setLogLeavl(int leavl)
{
    m_logLeavl = leavl;
}

void Logger::logMsg(QByteArray msg)
{
    m_file.write("\r\n");
    QString dateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss : ");
    m_file.write(dateTime.toUtf8());
    m_file.write(msg);
    m_file.flush();
}

void Logger::creatFile(){
    if (m_file.isOpen()){
        m_file.flush();
        m_file.close();
    }
    QString dateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd-HH-mm-ss");
    QDir dir(m_path);
    QString fileName = dir.absoluteFilePath(dateTime + ".log");
    m_file.setFileName(fileName);
    m_file.open(QIODevice::ReadWrite);
    m_file.write("/*****Logger Begin*****/");
    m_file.flush();
}

void Logger::MessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QByteArray localMsg = msg.toUtf8();

    Logger::self()->logMsg(localMsg);
    switch (type) {
    case QtDebugMsg:
    case QtInfoMsg:
        fprintf(stdout, msg.toLocal8Bit());
        fflush(stdout);
        break;
    case QtWarningMsg:
        fprintf(stderr, msg.toLocal8Bit());
        fflush(stderr);
        break;
    case QtFatalMsg:
        fprintf(stderr, msg.toLocal8Bit());
        fflush(stderr);
        abort();
    }
}


