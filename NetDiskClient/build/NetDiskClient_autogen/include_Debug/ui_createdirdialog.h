/********************************************************************************
** Form generated from reading UI file 'createdirdialog.ui'
**
** Created by: Qt User Interface Compiler version 6.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CREATEDIRDIALOG_H
#define UI_CREATEDIRDIALOG_H

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

class Ui_CreateDirDialog
{
public:
    QVBoxLayout *verticalLayout;
    QWidget *widget;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;
    QLineEdit *lineEdit;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *CreateDirDialog)
    {
        if (CreateDirDialog->objectName().isEmpty())
            CreateDirDialog->setObjectName("CreateDirDialog");
        CreateDirDialog->resize(341, 226);
        verticalLayout = new QVBoxLayout(CreateDirDialog);
        verticalLayout->setObjectName("verticalLayout");
        widget = new QWidget(CreateDirDialog);
        widget->setObjectName("widget");
        horizontalLayout_2 = new QHBoxLayout(widget);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        label = new QLabel(widget);
        label->setObjectName("label");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);
        QFont font;
        font.setPointSize(11);
        label->setFont(font);

        horizontalLayout_2->addWidget(label);

        lineEdit = new QLineEdit(widget);
        lineEdit->setObjectName("lineEdit");
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(lineEdit->sizePolicy().hasHeightForWidth());
        lineEdit->setSizePolicy(sizePolicy1);

        horizontalLayout_2->addWidget(lineEdit);


        verticalLayout->addWidget(widget);

        buttonBox = new QDialogButtonBox(CreateDirDialog);
        buttonBox->setObjectName("buttonBox");
        buttonBox->setOrientation(Qt::Orientation::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::StandardButton::Cancel|QDialogButtonBox::StandardButton::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(CreateDirDialog);
        QObject::connect(buttonBox, &QDialogButtonBox::accepted, CreateDirDialog, qOverload<>(&QDialog::accept));
        QObject::connect(buttonBox, &QDialogButtonBox::rejected, CreateDirDialog, qOverload<>(&QDialog::reject));

        QMetaObject::connectSlotsByName(CreateDirDialog);
    } // setupUi

    void retranslateUi(QDialog *CreateDirDialog)
    {
        CreateDirDialog->setWindowTitle(QCoreApplication::translate("CreateDirDialog", "Dialog", nullptr));
        label->setText(QCoreApplication::translate("CreateDirDialog", "\346\226\260\345\273\272\346\226\207\344\273\266\345\244\271\357\274\232", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CreateDirDialog: public Ui_CreateDirDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CREATEDIRDIALOG_H
