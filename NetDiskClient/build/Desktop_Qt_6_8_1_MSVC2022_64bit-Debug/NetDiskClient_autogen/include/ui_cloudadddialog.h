/********************************************************************************
** Form generated from reading UI file 'cloudadddialog.ui'
**
** Created by: Qt User Interface Compiler version 6.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CLOUDADDDIALOG_H
#define UI_CLOUDADDDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CloudAddDialog
{
public:
    QVBoxLayout *verticalLayout;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *shareLink;
    QWidget *widget_2;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QLineEdit *extractCode;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *CloudAddDialog)
    {
        if (CloudAddDialog->objectName().isEmpty())
            CloudAddDialog->setObjectName("CloudAddDialog");
        CloudAddDialog->resize(400, 300);
        verticalLayout = new QVBoxLayout(CloudAddDialog);
        verticalLayout->setObjectName("verticalLayout");
        widget = new QWidget(CloudAddDialog);
        widget->setObjectName("widget");
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setObjectName("horizontalLayout");
        label = new QLabel(widget);
        label->setObjectName("label");
        QFont font;
        font.setPointSize(13);
        label->setFont(font);
        label->setAlignment(Qt::AlignmentFlag::AlignLeading|Qt::AlignmentFlag::AlignLeft|Qt::AlignmentFlag::AlignVCenter);

        horizontalLayout->addWidget(label);

        shareLink = new QLineEdit(widget);
        shareLink->setObjectName("shareLink");

        horizontalLayout->addWidget(shareLink);


        verticalLayout->addWidget(widget);

        widget_2 = new QWidget(CloudAddDialog);
        widget_2->setObjectName("widget_2");
        horizontalLayout_2 = new QHBoxLayout(widget_2);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        label_2 = new QLabel(widget_2);
        label_2->setObjectName("label_2");
        label_2->setFont(font);

        horizontalLayout_2->addWidget(label_2);

        extractCode = new QLineEdit(widget_2);
        extractCode->setObjectName("extractCode");

        horizontalLayout_2->addWidget(extractCode);


        verticalLayout->addWidget(widget_2);

        buttonBox = new QDialogButtonBox(CloudAddDialog);
        buttonBox->setObjectName("buttonBox");
        buttonBox->setOrientation(Qt::Orientation::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::StandardButton::Cancel|QDialogButtonBox::StandardButton::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(CloudAddDialog);
        QObject::connect(buttonBox, &QDialogButtonBox::accepted, CloudAddDialog, qOverload<>(&QDialog::accept));
        QObject::connect(buttonBox, &QDialogButtonBox::rejected, CloudAddDialog, qOverload<>(&QDialog::reject));

        QMetaObject::connectSlotsByName(CloudAddDialog);
    } // setupUi

    void retranslateUi(QDialog *CloudAddDialog)
    {
        CloudAddDialog->setWindowTitle(QCoreApplication::translate("CloudAddDialog", "Dialog", nullptr));
        label->setText(QCoreApplication::translate("CloudAddDialog", "\345\234\260\345\235\200\357\274\232", nullptr));
        label_2->setText(QCoreApplication::translate("CloudAddDialog", "\346\217\220\345\217\226\347\240\201\357\274\232", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CloudAddDialog: public Ui_CloudAddDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CLOUDADDDIALOG_H
