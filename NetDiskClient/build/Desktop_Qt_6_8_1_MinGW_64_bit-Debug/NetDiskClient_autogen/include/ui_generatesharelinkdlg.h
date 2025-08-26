/********************************************************************************
** Form generated from reading UI file 'generatesharelinkdlg.ui'
**
** Created by: Qt User Interface Compiler version 6.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GENERATESHARELINKDLG_H
#define UI_GENERATESHARELINKDLG_H

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

class Ui_GenerateShareLinkDlg
{
public:
    QVBoxLayout *verticalLayout_2;
    QWidget *widget;
    QVBoxLayout *verticalLayout;
    QWidget *widget_2;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *shareLink;
    QWidget *widget_3;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QLineEdit *getCodeLine;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *GenerateShareLinkDlg)
    {
        if (GenerateShareLinkDlg->objectName().isEmpty())
            GenerateShareLinkDlg->setObjectName("GenerateShareLinkDlg");
        GenerateShareLinkDlg->resize(400, 300);
        verticalLayout_2 = new QVBoxLayout(GenerateShareLinkDlg);
        verticalLayout_2->setObjectName("verticalLayout_2");
        widget = new QWidget(GenerateShareLinkDlg);
        widget->setObjectName("widget");
        verticalLayout = new QVBoxLayout(widget);
        verticalLayout->setObjectName("verticalLayout");
        widget_2 = new QWidget(widget);
        widget_2->setObjectName("widget_2");
        horizontalLayout = new QHBoxLayout(widget_2);
        horizontalLayout->setObjectName("horizontalLayout");
        label = new QLabel(widget_2);
        label->setObjectName("label");
        QFont font;
        font.setPointSize(13);
        label->setFont(font);

        horizontalLayout->addWidget(label);

        shareLink = new QLineEdit(widget_2);
        shareLink->setObjectName("shareLink");

        horizontalLayout->addWidget(shareLink);


        verticalLayout->addWidget(widget_2);

        widget_3 = new QWidget(widget);
        widget_3->setObjectName("widget_3");
        horizontalLayout_2 = new QHBoxLayout(widget_3);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        label_2 = new QLabel(widget_3);
        label_2->setObjectName("label_2");
        label_2->setFont(font);

        horizontalLayout_2->addWidget(label_2);

        getCodeLine = new QLineEdit(widget_3);
        getCodeLine->setObjectName("getCodeLine");

        horizontalLayout_2->addWidget(getCodeLine);


        verticalLayout->addWidget(widget_3);


        verticalLayout_2->addWidget(widget);

        buttonBox = new QDialogButtonBox(GenerateShareLinkDlg);
        buttonBox->setObjectName("buttonBox");
        buttonBox->setOrientation(Qt::Orientation::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::StandardButton::Cancel|QDialogButtonBox::StandardButton::Ok);

        verticalLayout_2->addWidget(buttonBox);


        retranslateUi(GenerateShareLinkDlg);
        QObject::connect(buttonBox, &QDialogButtonBox::accepted, GenerateShareLinkDlg, qOverload<>(&QDialog::accept));
        QObject::connect(buttonBox, &QDialogButtonBox::rejected, GenerateShareLinkDlg, qOverload<>(&QDialog::reject));

        QMetaObject::connectSlotsByName(GenerateShareLinkDlg);
    } // setupUi

    void retranslateUi(QDialog *GenerateShareLinkDlg)
    {
        GenerateShareLinkDlg->setWindowTitle(QCoreApplication::translate("GenerateShareLinkDlg", "Dialog", nullptr));
        label->setText(QCoreApplication::translate("GenerateShareLinkDlg", "\351\223\276\346\216\245\357\274\232", nullptr));
        label_2->setText(QCoreApplication::translate("GenerateShareLinkDlg", "\346\217\220\345\217\226\347\240\201\357\274\232", nullptr));
    } // retranslateUi

};

namespace Ui {
    class GenerateShareLinkDlg: public Ui_GenerateShareLinkDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GENERATESHARELINKDLG_H
