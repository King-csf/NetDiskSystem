#include "createdirdialog.h"
#include "ui_createdirdialog.h"

CreateDirDialog::CreateDirDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::CreateDirDialog)
{
    ui->setupUi(this);

    this->setWindowTitle("新建文件夹");
}

CreateDirDialog::~CreateDirDialog()
{
    delete ui;
}

QString CreateDirDialog::getEditLineText()
{
    return ui->lineEdit->text();
}
