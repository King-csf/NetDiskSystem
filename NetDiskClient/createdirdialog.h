#ifndef CREATEDIRDIALOG_H
#define CREATEDIRDIALOG_H

#include <QDialog>

namespace Ui {
class CreateDirDialog;
}

class CreateDirDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CreateDirDialog(QWidget *parent = nullptr);
    ~CreateDirDialog();

    QString getEditLineText();

private:
    Ui::CreateDirDialog *ui;
};

#endif // CREATEDIRDIALOG_H
