#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "NetworkManager.h"
#include "FileManager.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


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
    NetworkManager::self()->post("http://139.224.194.190:8088/login"
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
    NetworkManager::self()->post("http://139.224.194.190:8088/login"
                                 ,map)->responseString([](const bool scucess, const QString &string){
        if(scucess){
            qDebug() << "client succeedCallback: result:" <<string;
        }
    });

    FileManager::self()->getImage("https://static.pgyer.com/static-20161125/assets/img/footer_service.png",[=](const bool success, const QImage &image){
        if (image.isNull()) {
            ui->label->setText("no image");
        }
        else {
            ui->label->setPixmap(QPixmap::fromImage(image));
        }
    });


}

MainWindow::~MainWindow()
{
    delete ui;
}
