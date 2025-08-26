#include "generatesharelinkdlg.h"
#include "ui_generatesharelinkdlg.h"

GenerateShareLinkDlg::GenerateShareLinkDlg(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::GenerateShareLinkDlg)
{
    ui->setupUi(this);
}

GenerateShareLinkDlg::~GenerateShareLinkDlg()
{
    delete ui;
}

void GenerateShareLinkDlg::setLink(QString shareLink)
{
    ui->shareLink->setText(shareLink);
}

void GenerateShareLinkDlg::setGetCode(QString code)
{
    ui->getCodeLine->setText(code);
}
