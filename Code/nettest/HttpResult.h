#ifndef HTTPRESULT_H
#define HTTPRESULT_H

#include <QObject>

class HttpResult : public QObject
{
    Q_OBJECT
public:
    explicit HttpResult(QObject *parent = 0);

signals:

public slots:
};

#endif // HTTPRESULT_H