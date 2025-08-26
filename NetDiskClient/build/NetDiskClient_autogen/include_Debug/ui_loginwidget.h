/********************************************************************************
** Form generated from reading UI file 'loginwidget.ui'
**
** Created by: Qt User Interface Compiler version 6.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGINWIDGET_H
#define UI_LOGINWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_loginwidget
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout_2;
    QWidget *widget_4;
    QVBoxLayout *verticalLayout;
    QWidget *widget_7;
    QHBoxLayout *horizontalLayout_6;
    QSpacerItem *horizontalSpacer_12;
    QWidget *widget_9;
    QHBoxLayout *horizontalLayout_8;
    QSpacerItem *horizontalSpacer;
    QLabel *label;
    QSpacerItem *horizontalSpacer_2;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QLineEdit *count_line;
    QWidget *widget_2;
    QHBoxLayout *horizontalLayout_2;
    QLineEdit *password_line;
    QWidget *widget_6;
    QHBoxLayout *horizontalLayout_5;
    QWidget *widget_5;
    QHBoxLayout *horizontalLayout_4;
    QPushButton *login_btn;
    QWidget *widget_3;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer_9;
    QPushButton *sigin_up_btn;
    QSpacerItem *horizontalSpacer_11;
    QPushButton *forget_passwd_btn;
    QSpacerItem *horizontalSpacer_10;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *loginwidget)
    {
        if (loginwidget->objectName().isEmpty())
            loginwidget->setObjectName("loginwidget");
        loginwidget->resize(380, 473);
        centralwidget = new QWidget(loginwidget);
        centralwidget->setObjectName("centralwidget");
        verticalLayout_2 = new QVBoxLayout(centralwidget);
        verticalLayout_2->setObjectName("verticalLayout_2");
        widget_4 = new QWidget(centralwidget);
        widget_4->setObjectName("widget_4");
        verticalLayout = new QVBoxLayout(widget_4);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(-1, 0, 0, -1);
        widget_7 = new QWidget(widget_4);
        widget_7->setObjectName("widget_7");
        horizontalLayout_6 = new QHBoxLayout(widget_7);
        horizontalLayout_6->setSpacing(0);
        horizontalLayout_6->setObjectName("horizontalLayout_6");
        horizontalLayout_6->setContentsMargins(0, 0, 0, 0);
        horizontalSpacer_12 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_12);


        verticalLayout->addWidget(widget_7);

        widget_9 = new QWidget(widget_4);
        widget_9->setObjectName("widget_9");
        horizontalLayout_8 = new QHBoxLayout(widget_9);
        horizontalLayout_8->setObjectName("horizontalLayout_8");
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_8->addItem(horizontalSpacer);

        label = new QLabel(widget_9);
        label->setObjectName("label");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);
        label->setMinimumSize(QSize(0, 0));
        label->setStyleSheet(QString::fromUtf8("\n"
"border-radius: 45px;\n"
"background-color: rgb(255, 255, 255);\n"
""));
        label->setPixmap(QPixmap(QString::fromUtf8(":/new/prefix1/src/favicon(1).jpg")));
        label->setAlignment(Qt::AlignmentFlag::AlignCenter);

        horizontalLayout_8->addWidget(label);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_8->addItem(horizontalSpacer_2);


        verticalLayout->addWidget(widget_9);

        widget = new QWidget(widget_4);
        widget->setObjectName("widget");
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setObjectName("horizontalLayout");
        count_line = new QLineEdit(widget);
        count_line->setObjectName("count_line");
        sizePolicy.setHeightForWidth(count_line->sizePolicy().hasHeightForWidth());
        count_line->setSizePolicy(sizePolicy);
        QFont font;
        font.setPointSize(11);
        count_line->setFont(font);
        count_line->setStyleSheet(QString::fromUtf8("height:50;\n"
"width:250;\n"
"border:none;\n"
"border-radius:10;"));
        count_line->setAlignment(Qt::AlignmentFlag::AlignCenter);

        horizontalLayout->addWidget(count_line);


        verticalLayout->addWidget(widget);

        widget_2 = new QWidget(widget_4);
        widget_2->setObjectName("widget_2");
        horizontalLayout_2 = new QHBoxLayout(widget_2);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        password_line = new QLineEdit(widget_2);
        password_line->setObjectName("password_line");
        sizePolicy.setHeightForWidth(password_line->sizePolicy().hasHeightForWidth());
        password_line->setSizePolicy(sizePolicy);
        password_line->setFont(font);
        password_line->setStyleSheet(QString::fromUtf8("height:50;\n"
"width:250;\n"
"border:none;\n"
"border-radius:10;"));
        password_line->setEchoMode(QLineEdit::EchoMode::Password);
        password_line->setAlignment(Qt::AlignmentFlag::AlignCenter);

        horizontalLayout_2->addWidget(password_line);


        verticalLayout->addWidget(widget_2);


        verticalLayout_2->addWidget(widget_4);

        widget_6 = new QWidget(centralwidget);
        widget_6->setObjectName("widget_6");
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(widget_6->sizePolicy().hasHeightForWidth());
        widget_6->setSizePolicy(sizePolicy1);
        horizontalLayout_5 = new QHBoxLayout(widget_6);
        horizontalLayout_5->setObjectName("horizontalLayout_5");

        verticalLayout_2->addWidget(widget_6);

        widget_5 = new QWidget(centralwidget);
        widget_5->setObjectName("widget_5");
        sizePolicy1.setHeightForWidth(widget_5->sizePolicy().hasHeightForWidth());
        widget_5->setSizePolicy(sizePolicy1);
        horizontalLayout_4 = new QHBoxLayout(widget_5);
        horizontalLayout_4->setObjectName("horizontalLayout_4");
        horizontalLayout_4->setContentsMargins(5, 5, 5, 5);
        login_btn = new QPushButton(widget_5);
        login_btn->setObjectName("login_btn");
        login_btn->setEnabled(true);
        sizePolicy.setHeightForWidth(login_btn->sizePolicy().hasHeightForWidth());
        login_btn->setSizePolicy(sizePolicy);
        login_btn->setStyleSheet(QString::fromUtf8("border:none;\n"
"background-color: rgb(0, 170, 255);\n"
"width:200;\n"
"height:30;"));

        horizontalLayout_4->addWidget(login_btn);


        verticalLayout_2->addWidget(widget_5);

        widget_3 = new QWidget(centralwidget);
        widget_3->setObjectName("widget_3");
        horizontalLayout_3 = new QHBoxLayout(widget_3);
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        horizontalSpacer_9 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_9);

        sigin_up_btn = new QPushButton(widget_3);
        sigin_up_btn->setObjectName("sigin_up_btn");
        QFont font1;
        font1.setUnderline(true);
        sigin_up_btn->setFont(font1);
        sigin_up_btn->setStyleSheet(QString::fromUtf8("color: rgb(0, 85, 255);"));

        horizontalLayout_3->addWidget(sigin_up_btn);

        horizontalSpacer_11 = new QSpacerItem(20, 20, QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_11);

        forget_passwd_btn = new QPushButton(widget_3);
        forget_passwd_btn->setObjectName("forget_passwd_btn");
        forget_passwd_btn->setFont(font1);
        forget_passwd_btn->setStyleSheet(QString::fromUtf8("color: rgb(0, 85, 255);"));

        horizontalLayout_3->addWidget(forget_passwd_btn);

        horizontalSpacer_10 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_10);


        verticalLayout_2->addWidget(widget_3);

        loginwidget->setCentralWidget(centralwidget);
        menubar = new QMenuBar(loginwidget);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 380, 18));
        loginwidget->setMenuBar(menubar);
        statusbar = new QStatusBar(loginwidget);
        statusbar->setObjectName("statusbar");
        loginwidget->setStatusBar(statusbar);

        retranslateUi(loginwidget);

        QMetaObject::connectSlotsByName(loginwidget);
    } // setupUi

    void retranslateUi(QMainWindow *loginwidget)
    {
        loginwidget->setWindowTitle(QCoreApplication::translate("loginwidget", "loginwidget", nullptr));
        label->setText(QString());
        count_line->setText(QString());
        count_line->setPlaceholderText(QCoreApplication::translate("loginwidget", "\350\257\267\350\276\223\345\205\245\350\264\246\345\217\267", nullptr));
        password_line->setInputMask(QString());
        password_line->setText(QString());
        password_line->setPlaceholderText(QCoreApplication::translate("loginwidget", "\350\257\267\350\276\223\345\205\245\345\257\206\347\240\201", nullptr));
        login_btn->setText(QCoreApplication::translate("loginwidget", "\347\231\273\345\275\225", nullptr));
        sigin_up_btn->setText(QCoreApplication::translate("loginwidget", "\346\263\250\345\206\214", nullptr));
        forget_passwd_btn->setText(QCoreApplication::translate("loginwidget", "\345\277\230\350\256\260\345\257\206\347\240\201", nullptr));
    } // retranslateUi

};

namespace Ui {
    class loginwidget: public Ui_loginwidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGINWIDGET_H
