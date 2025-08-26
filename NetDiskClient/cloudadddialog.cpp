#include "cloudadddialog.h"
#include "ui_cloudadddialog.h"

CloudAddDialog::CloudAddDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::CloudAddDialog)
{
    ui->setupUi(this);
}

CloudAddDialog::~CloudAddDialog()
{
    delete ui;
}

QString CloudAddDialog::getAddr()
{
    return ui->shareLink->text();
}

QString CloudAddDialog::getExtractCode()
{
    return ui->extractCode->text();
}
