#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <QMainWindow>
#include <QtHttpServer/QHttpServer>
 #include <QNetworkRequest>
#include <QJsonObject>
 #include <QJsonDocument>
 #include <QNetworkReply>
#include "mainwindow.h"


QT_BEGIN_NAMESPACE
namespace Ui {
class loginwidget;
}
QT_END_NAMESPACE

class loginwidget : public QMainWindow
{
    Q_OBJECT

public:
    loginwidget(QWidget *parent = nullptr);
    ~loginwidget();

private slots:
    void on_login_btn_clicked();

private:
    Ui::loginwidget *ui;
    QNetworkAccessManager *manager;
    QString sUrl ;
};
#endif // LOGINWIDGET_H
