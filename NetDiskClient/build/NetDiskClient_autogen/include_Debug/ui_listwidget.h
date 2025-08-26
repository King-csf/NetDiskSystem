/********************************************************************************
** Form generated from reading UI file 'listwidget.ui'
**
** Created by: Qt User Interface Compiler version 6.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LISTWIDGET_H
#define UI_LISTWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <mytreeview.h>

QT_BEGIN_NAMESPACE

class Ui_ListWidget
{
public:
    QVBoxLayout *verticalLayout;
    QToolButton *toolButton;
    MyTreeView *treeView;

    void setupUi(QWidget *ListWidget)
    {
        if (ListWidget->objectName().isEmpty())
            ListWidget->setObjectName("ListWidget");
        ListWidget->resize(414, 314);
        verticalLayout = new QVBoxLayout(ListWidget);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        toolButton = new QToolButton(ListWidget);
        toolButton->setObjectName("toolButton");
        toolButton->setStyleSheet(QString::fromUtf8("border:none;"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/new/prefix1/image/\350\277\224\345\233\236-copy.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        toolButton->setIcon(icon);
        toolButton->setIconSize(QSize(25, 25));

        verticalLayout->addWidget(toolButton);

        treeView = new MyTreeView(ListWidget);
        treeView->setObjectName("treeView");

        verticalLayout->addWidget(treeView);


        retranslateUi(ListWidget);

        QMetaObject::connectSlotsByName(ListWidget);
    } // setupUi

    void retranslateUi(QWidget *ListWidget)
    {
        ListWidget->setWindowTitle(QCoreApplication::translate("ListWidget", "Form", nullptr));
        toolButton->setText(QCoreApplication::translate("ListWidget", "...", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ListWidget: public Ui_ListWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LISTWIDGET_H
