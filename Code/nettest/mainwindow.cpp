#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "HttpManager.h"
#include "FileManager.h"
#include "Logger.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QTimer::singleShot(0,[=](){
        this->test();
    });


    Logger::self()->setLogLevel(LoggerDebug);

    startTimer(100);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::test(){
    //    auto succeedCallback = []
    //            (
    //                const QJsonDocument &json
    //            )
    //    {
    //        QJsonObject jobject = json.object();
    //        int result = jobject.find("result").value().toVariant().toInt();
    //        QString msg = jobject.find("msg").value().toString();
    //        if (result == 0) {
    //            QJsonObject data = jobject.find("data").value().toObject();
    //            QString id = data.find("id").value().toVariant().toString();
    //            QString nickName = data.find("nickName").value().toString();
    //            qDebug() << "client succeedCallback: result:" << result <<"msg:"<<msg
    //                     <<"id:"<<id<<"nickName:"<<nickName;
    //        }
    //    };

        QVariantMap map;
        map["userName"] = "18672788959";
        map["password"] = "18672788959";
        HttpManager::self()->post("http://139.224.194.190:8088/login"
                                     ,map)->responseJSON([](const bool scucess, const QJsonDocument &json){
            if(scucess){
                QJsonObject jobject = json.object();
                int result = jobject.find("result").value().toVariant().toInt();
                QString msg = jobject.find("msg").value().toString();
                if (result == 0) {
                    QJsonObject data = jobject.find("data").value().toObject();
                    QString id = data.find("id").value().toVariant().toString();
                    QString nickName = data.find("nickName").value().toString();
                    qDebug() << "client succeedCallback: result:" << result <<"msg:"<<msg
                             <<"id:"<<id<<"nickName:"<<nickName;
                }
            }
        });

        map["password"] = "186727889591";
        HttpManager::self()->post("http://139.224.194.190:8088/login"
                                     ,map)->responseString([](const bool scucess, const QString &string){
            if(scucess){
                qDebug() << "client succeedCallback: result:" <<string;
            }
        });

        FileManager::self()->getImage("http://img.ivsky.com/img/bizhi/pre/201611/17/i_pace-001.jpg",[=](const bool success, const QImage &image){
            if (image.isNull()) {
                ui->label->setText("no image");
            }
            else {
                ui->label->setPixmap(QPixmap::fromImage(image));
            }
        });

        QVariantMap map2;
        map2["page"] = 1;
        map2["rows"] = 2;

        HttpManager::self()->get("http://139.224.194.190:8088/getAllUsersE"
                                     ,map2)->responseString([](const bool scucess, const QString &string){
            if(scucess){
                qDebug() << "client succeedCallback: result:" <<string;
            }
        });
}

void MainWindow::timerEvent(QTimerEvent *event)
{
}
