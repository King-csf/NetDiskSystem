/********************************************************************************
** Form generated from reading UI file 'player.ui'
**
** Created by: Qt User Interface Compiler version 6.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PLAYER_H
#define UI_PLAYER_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Player
{
public:
    QVBoxLayout *verticalLayout;
    QWidget *SDL_Widget;
    QWidget *widget_2;
    QHBoxLayout *horizontalLayout_2;
    QLabel *now_time;
    QSlider *progress_bar;
    QLabel *total_time;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QComboBox *speedComboBox;
    QToolButton *last_btn;
    QToolButton *start_btn;
    QToolButton *stop_btn;
    QToolButton *next_btn;
    QSpacerItem *horizontalSpacer_2;
    QToolButton *toolButton_3;
    QSlider *sound_bar;

    void setupUi(QWidget *Player)
    {
        if (Player->objectName().isEmpty())
            Player->setObjectName("Player");
        Player->resize(589, 419);
        Player->setStyleSheet(QString::fromUtf8("background-color: rgb(55, 55, 55);\n"
"color: rgb(255, 255, 255);\n"
"MainWindow{\n"
"border:3px solid #000000;\n"
"}"));
        verticalLayout = new QVBoxLayout(Player);
        verticalLayout->setObjectName("verticalLayout");
        SDL_Widget = new QWidget(Player);
        SDL_Widget->setObjectName("SDL_Widget");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(SDL_Widget->sizePolicy().hasHeightForWidth());
        SDL_Widget->setSizePolicy(sizePolicy);

        verticalLayout->addWidget(SDL_Widget);

        widget_2 = new QWidget(Player);
        widget_2->setObjectName("widget_2");
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(widget_2->sizePolicy().hasHeightForWidth());
        widget_2->setSizePolicy(sizePolicy1);
        horizontalLayout_2 = new QHBoxLayout(widget_2);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        now_time = new QLabel(widget_2);
        now_time->setObjectName("now_time");
        now_time->setTextFormat(Qt::TextFormat::MarkdownText);

        horizontalLayout_2->addWidget(now_time);

        progress_bar = new QSlider(widget_2);
        progress_bar->setObjectName("progress_bar");
        progress_bar->setOrientation(Qt::Orientation::Horizontal);

        horizontalLayout_2->addWidget(progress_bar);

        total_time = new QLabel(widget_2);
        total_time->setObjectName("total_time");

        horizontalLayout_2->addWidget(total_time);


        verticalLayout->addWidget(widget_2);

        widget = new QWidget(Player);
        widget->setObjectName("widget");
        sizePolicy1.setHeightForWidth(widget->sizePolicy().hasHeightForWidth());
        widget->setSizePolicy(sizePolicy1);
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        speedComboBox = new QComboBox(widget);
        speedComboBox->addItem(QString());
        speedComboBox->addItem(QString());
        speedComboBox->addItem(QString());
        speedComboBox->addItem(QString());
        speedComboBox->addItem(QString());
        speedComboBox->addItem(QString());
        speedComboBox->setObjectName("speedComboBox");

        horizontalLayout->addWidget(speedComboBox);

        last_btn = new QToolButton(widget);
        last_btn->setObjectName("last_btn");
        last_btn->setStyleSheet(QString::fromUtf8("border:none;"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/new/prefix1/image/last.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        last_btn->setIcon(icon);
        last_btn->setIconSize(QSize(20, 20));

        horizontalLayout->addWidget(last_btn);

        start_btn = new QToolButton(widget);
        start_btn->setObjectName("start_btn");
        start_btn->setStyleSheet(QString::fromUtf8("border:none;"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/new/prefix1/image/\346\222\255\346\224\276.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        start_btn->setIcon(icon1);
        start_btn->setIconSize(QSize(20, 20));

        horizontalLayout->addWidget(start_btn);

        stop_btn = new QToolButton(widget);
        stop_btn->setObjectName("stop_btn");
        stop_btn->setStyleSheet(QString::fromUtf8("border:none;"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/new/prefix1/image/\346\255\243\346\226\271\345\275\242.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        stop_btn->setIcon(icon2);
        stop_btn->setIconSize(QSize(20, 20));

        horizontalLayout->addWidget(stop_btn);

        next_btn = new QToolButton(widget);
        next_btn->setObjectName("next_btn");
        next_btn->setStyleSheet(QString::fromUtf8("border:none;"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/new/prefix1/image/next.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        next_btn->setIcon(icon3);
        next_btn->setIconSize(QSize(20, 20));

        horizontalLayout->addWidget(next_btn);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        toolButton_3 = new QToolButton(widget);
        toolButton_3->setObjectName("toolButton_3");
        toolButton_3->setStyleSheet(QString::fromUtf8("border:none;"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/new/prefix1/image/\351\237\263\351\207\217.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        toolButton_3->setIcon(icon4);
        toolButton_3->setIconSize(QSize(20, 20));

        horizontalLayout->addWidget(toolButton_3);

        sound_bar = new QSlider(widget);
        sound_bar->setObjectName("sound_bar");
        sound_bar->setOrientation(Qt::Orientation::Horizontal);

        horizontalLayout->addWidget(sound_bar);


        verticalLayout->addWidget(widget);


        retranslateUi(Player);

        QMetaObject::connectSlotsByName(Player);
    } // setupUi

    void retranslateUi(QWidget *Player)
    {
        Player->setWindowTitle(QCoreApplication::translate("Player", "Form", nullptr));
        now_time->setText(QCoreApplication::translate("Player", "00:00:00", nullptr));
        total_time->setText(QCoreApplication::translate("Player", "00.00.00", nullptr));
        speedComboBox->setItemText(0, QCoreApplication::translate("Player", "2.0x", nullptr));
        speedComboBox->setItemText(1, QCoreApplication::translate("Player", "1.5x", nullptr));
        speedComboBox->setItemText(2, QCoreApplication::translate("Player", "1.25x", nullptr));
        speedComboBox->setItemText(3, QCoreApplication::translate("Player", "1.0x", nullptr));
        speedComboBox->setItemText(4, QCoreApplication::translate("Player", "0.75x", nullptr));
        speedComboBox->setItemText(5, QCoreApplication::translate("Player", "0.5x", nullptr));

        last_btn->setText(QCoreApplication::translate("Player", "...", nullptr));
        start_btn->setText(QCoreApplication::translate("Player", "...", nullptr));
        stop_btn->setText(QCoreApplication::translate("Player", "...", nullptr));
        next_btn->setText(QCoreApplication::translate("Player", "...", nullptr));
        toolButton_3->setText(QCoreApplication::translate("Player", "...", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Player: public Ui_Player {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PLAYER_H
