/********************************************************************************
** Form generated from reading UI file 'transferwidget.ui'
**
** Created by: Qt User Interface Compiler version 6.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TRANSFERWIDGET_H
#define UI_TRANSFERWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TransferWidget
{
public:
    QHBoxLayout *horizontalLayout;
    QTreeView *treeView;

    void setupUi(QWidget *TransferWidget)
    {
        if (TransferWidget->objectName().isEmpty())
            TransferWidget->setObjectName("TransferWidget");
        TransferWidget->resize(481, 349);
        horizontalLayout = new QHBoxLayout(TransferWidget);
        horizontalLayout->setObjectName("horizontalLayout");
        treeView = new QTreeView(TransferWidget);
        treeView->setObjectName("treeView");

        horizontalLayout->addWidget(treeView);


        retranslateUi(TransferWidget);

        QMetaObject::connectSlotsByName(TransferWidget);
    } // setupUi

    void retranslateUi(QWidget *TransferWidget)
    {
        TransferWidget->setWindowTitle(QCoreApplication::translate("TransferWidget", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TransferWidget: public Ui_TransferWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TRANSFERWIDGET_H
