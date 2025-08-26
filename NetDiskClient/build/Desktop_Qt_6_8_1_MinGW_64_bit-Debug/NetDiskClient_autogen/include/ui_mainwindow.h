/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_mainwindow
{
public:
    QVBoxLayout *verticalLayout_2;
    QWidget *widget_2;
    QHBoxLayout *horizontalLayout;
    QPushButton *uploadFile;
    QPushButton *uploadDir;
    QPushButton *createDir;
    QPushButton *cloudAdd;
    QSpacerItem *horizontalSpacer;
    QPushButton *stopBtn;
    QPushButton *continueBtn;
    QHBoxLayout *horizontalLayout_2;
    QWidget *widget;
    QVBoxLayout *verticalLayout;
    QListWidget *listWidget;
    QWidget *widget_3;
    QVBoxLayout *verticalLayout_3;
    QStackedWidget *stackedWidget;
    QWidget *page;
    QWidget *page_2;

    void setupUi(QWidget *mainwindow)
    {
        if (mainwindow->objectName().isEmpty())
            mainwindow->setObjectName("mainwindow");
        mainwindow->resize(916, 580);
        mainwindow->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        verticalLayout_2 = new QVBoxLayout(mainwindow);
        verticalLayout_2->setObjectName("verticalLayout_2");
        widget_2 = new QWidget(mainwindow);
        widget_2->setObjectName("widget_2");
        horizontalLayout = new QHBoxLayout(widget_2);
        horizontalLayout->setObjectName("horizontalLayout");
        uploadFile = new QPushButton(widget_2);
        uploadFile->setObjectName("uploadFile");
        uploadFile->setStyleSheet(QString::fromUtf8("border: none;\n"
"    background-color: rgb(85, 170, 255);\n"
"    color: rgb(255, 255, 255);\n"
"    min-height: 30px;\n"
"    min-width: 80px;\n"
"    border-radius: 15px; "));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/new/prefix1/image/\344\270\212\344\274\240\346\226\207\344\273\266.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        uploadFile->setIcon(icon);

        horizontalLayout->addWidget(uploadFile);

        uploadDir = new QPushButton(widget_2);
        uploadDir->setObjectName("uploadDir");
        uploadDir->setStyleSheet(QString::fromUtf8("border: none;\n"
"    background-color: rgb(85, 170, 255);\n"
"    color: rgb(255, 255, 255);\n"
"    min-height: 30px;\n"
"    min-width: 80px;\n"
"    border-radius: 15px; "));
        uploadDir->setIcon(icon);

        horizontalLayout->addWidget(uploadDir);

        createDir = new QPushButton(widget_2);
        createDir->setObjectName("createDir");
        createDir->setStyleSheet(QString::fromUtf8("border: none;\n"
"background-color: rgb(236, 244, 255);\n"
"color: rgb(85, 85, 255);\n"
"    min-height: 30px;\n"
"    min-width: 80px;\n"
"   border-top-left-radius: 15px;\n"
"\n"
"border-bottom-left-radius: 15px;"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/new/prefix1/image/\346\226\260\345\273\272\346\226\207\344\273\266\345\244\271.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        createDir->setIcon(icon1);

        horizontalLayout->addWidget(createDir);

        cloudAdd = new QPushButton(widget_2);
        cloudAdd->setObjectName("cloudAdd");
        cloudAdd->setStyleSheet(QString::fromUtf8("border: none;\n"
"background-color: rgb(236, 244, 255);\n"
"color: rgb(85, 85, 255);\n"
"    min-height: 30px;\n"
"    min-width: 80px;\n"
"   border-top-right-radius: 15px;\n"
"\n"
"border-bottom-right-radius: 15px;"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/new/prefix1/image/\344\272\221\345\257\271\350\261\241\345\255\230\345\202\250.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        cloudAdd->setIcon(icon2);

        horizontalLayout->addWidget(cloudAdd);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        stopBtn = new QPushButton(widget_2);
        stopBtn->setObjectName("stopBtn");

        horizontalLayout->addWidget(stopBtn);

        continueBtn = new QPushButton(widget_2);
        continueBtn->setObjectName("continueBtn");

        horizontalLayout->addWidget(continueBtn);


        verticalLayout_2->addWidget(widget_2);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        widget = new QWidget(mainwindow);
        widget->setObjectName("widget");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Maximum, QSizePolicy::Policy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(widget->sizePolicy().hasHeightForWidth());
        widget->setSizePolicy(sizePolicy);
        verticalLayout = new QVBoxLayout(widget);
        verticalLayout->setObjectName("verticalLayout");
        listWidget = new QListWidget(widget);
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/new/prefix1/image/\346\226\207\344\273\266.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        QFont font;
        font.setPointSize(14);
        QListWidgetItem *__qlistwidgetitem = new QListWidgetItem(listWidget);
        __qlistwidgetitem->setFont(font);
        __qlistwidgetitem->setIcon(icon3);
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/new/prefix1/image/\350\247\206\351\242\221.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        QListWidgetItem *__qlistwidgetitem1 = new QListWidgetItem(listWidget);
        __qlistwidgetitem1->setFont(font);
        __qlistwidgetitem1->setIcon(icon4);
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/new/prefix1/image/\345\233\276\347\211\207.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        QListWidgetItem *__qlistwidgetitem2 = new QListWidgetItem(listWidget);
        __qlistwidgetitem2->setFont(font);
        __qlistwidgetitem2->setIcon(icon5);
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/new/prefix1/image/\346\226\207\346\241\243.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        QListWidgetItem *__qlistwidgetitem3 = new QListWidgetItem(listWidget);
        __qlistwidgetitem3->setFont(font);
        __qlistwidgetitem3->setIcon(icon6);
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/new/prefix1/image/\345\205\266\344\273\226\346\226\207\344\273\266.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        QListWidgetItem *__qlistwidgetitem4 = new QListWidgetItem(listWidget);
        __qlistwidgetitem4->setFont(font);
        __qlistwidgetitem4->setIcon(icon7);
        QListWidgetItem *__qlistwidgetitem5 = new QListWidgetItem(listWidget);
        __qlistwidgetitem5->setFont(font);
        QListWidgetItem *__qlistwidgetitem6 = new QListWidgetItem(listWidget);
        __qlistwidgetitem6->setFont(font);
        listWidget->setObjectName("listWidget");
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Maximum, QSizePolicy::Policy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(listWidget->sizePolicy().hasHeightForWidth());
        listWidget->setSizePolicy(sizePolicy1);
        listWidget->setStyleSheet(QString::fromUtf8("width:100;"));
        listWidget->setLineWidth(1);

        verticalLayout->addWidget(listWidget);


        horizontalLayout_2->addWidget(widget);

        widget_3 = new QWidget(mainwindow);
        widget_3->setObjectName("widget_3");
        verticalLayout_3 = new QVBoxLayout(widget_3);
        verticalLayout_3->setObjectName("verticalLayout_3");
        stackedWidget = new QStackedWidget(widget_3);
        stackedWidget->setObjectName("stackedWidget");
        page = new QWidget();
        page->setObjectName("page");
        stackedWidget->addWidget(page);
        page_2 = new QWidget();
        page_2->setObjectName("page_2");
        stackedWidget->addWidget(page_2);

        verticalLayout_3->addWidget(stackedWidget);


        horizontalLayout_2->addWidget(widget_3);


        verticalLayout_2->addLayout(horizontalLayout_2);


        retranslateUi(mainwindow);

        QMetaObject::connectSlotsByName(mainwindow);
    } // setupUi

    void retranslateUi(QWidget *mainwindow)
    {
        mainwindow->setWindowTitle(QCoreApplication::translate("mainwindow", "Form", nullptr));
        uploadFile->setText(QCoreApplication::translate("mainwindow", "\344\270\212\344\274\240\346\226\207\344\273\266", nullptr));
        uploadDir->setText(QCoreApplication::translate("mainwindow", "\344\270\212\344\274\240\346\226\207\344\273\266\345\244\271", nullptr));
        createDir->setText(QCoreApplication::translate("mainwindow", "\346\226\260\345\273\272\346\226\207\344\273\266\345\244\271", nullptr));
        cloudAdd->setText(QCoreApplication::translate("mainwindow", "\344\272\221\346\267\273\345\212\240", nullptr));
        stopBtn->setText(QCoreApplication::translate("mainwindow", "\346\232\202\345\201\234", nullptr));
        continueBtn->setText(QCoreApplication::translate("mainwindow", "\347\273\247\347\273\255", nullptr));

        const bool __sortingEnabled = listWidget->isSortingEnabled();
        listWidget->setSortingEnabled(false);
        QListWidgetItem *___qlistwidgetitem = listWidget->item(0);
        ___qlistwidgetitem->setText(QCoreApplication::translate("mainwindow", "\346\210\221\347\232\204\346\226\207\344\273\266", nullptr));
        QListWidgetItem *___qlistwidgetitem1 = listWidget->item(1);
        ___qlistwidgetitem1->setText(QCoreApplication::translate("mainwindow", "\350\247\206\351\242\221", nullptr));
        QListWidgetItem *___qlistwidgetitem2 = listWidget->item(2);
        ___qlistwidgetitem2->setText(QCoreApplication::translate("mainwindow", "\345\233\276\347\211\207", nullptr));
        QListWidgetItem *___qlistwidgetitem3 = listWidget->item(3);
        ___qlistwidgetitem3->setText(QCoreApplication::translate("mainwindow", "\346\226\207\346\241\243", nullptr));
        QListWidgetItem *___qlistwidgetitem4 = listWidget->item(4);
        ___qlistwidgetitem4->setText(QCoreApplication::translate("mainwindow", "\345\205\266\344\273\226", nullptr));
        QListWidgetItem *___qlistwidgetitem5 = listWidget->item(5);
        ___qlistwidgetitem5->setText(QCoreApplication::translate("mainwindow", "\344\270\212\344\274\240", nullptr));
        QListWidgetItem *___qlistwidgetitem6 = listWidget->item(6);
        ___qlistwidgetitem6->setText(QCoreApplication::translate("mainwindow", "\344\270\213\350\275\275", nullptr));
        listWidget->setSortingEnabled(__sortingEnabled);

    } // retranslateUi

};

namespace Ui {
    class mainwindow: public Ui_mainwindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
