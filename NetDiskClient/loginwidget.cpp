#include "loginwidget.h"
#include "./ui_loginwidget.h"

loginwidget::loginwidget(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::loginwidget)
{
    ui->setupUi(this);
    manager = new QNetworkAccessManager(this);

}

loginwidget::~loginwidget()
{
    delete ui;
}

void loginwidget::on_login_btn_clicked()
{
    QNetworkRequest request(QUrl("http://192.168.111.200:8888/login"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QJsonObject json;
    QString account = ui->count_line->text();
    QString password = ui->password_line->text();
    json["account"] = account;
    json["password"] = password;
    QByteArray jsonData = QJsonDocument(json).toJson();

    qDebug() << jsonData;

    QNetworkReply *reply = manager->post(request, jsonData);

    connect(reply, &QNetworkReply::finished, this, [this,account](){

        QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());

        //状态码
        int httpStatus = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        qDebug() << "[DEBUG]状态码：" << httpStatus;
        if (reply->error() == QNetworkReply::NoError)
        {

            QByteArray response = reply->readAll();
            //qDebug() << "Response:" << response;

            if(httpStatus != 401)
            {
                mainwindow * mw = new mainwindow(QString::fromStdString(response.toStdString()),account);
                mw->setAttribute(Qt::WA_DeleteOnClose);
                mw->show();
                this->close();
                //this->deleteLater();
            }

        }
        else
        {
            qDebug() << "Error:" << reply->errorString();
            //网络错误
            if(httpStatus == 0)
            {
                QMessageBox::critical(
                    nullptr,          // 父窗口
                    "网络错误",           // 标题
                    "请检查网络连接！"      // 内容
                    );
            }
            QMessageBox::critical(
                nullptr,          // 父窗口
                "登录错误",           // 标题
                "账号或密码错误！"      // 内容
                );

        }
        reply->deleteLater();
    });
}

