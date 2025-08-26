#ifndef CLOUDADDDIALOG_H
#define CLOUDADDDIALOG_H

#include <QDialog>

namespace Ui {
class CloudAddDialog;
}

class CloudAddDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CloudAddDialog(QWidget *parent = nullptr);
    ~CloudAddDialog();

    QString getAddr();
    QString getExtractCode();

private:
    Ui::CloudAddDialog *ui;
};

#endif // CLOUDADDDIALOG_H
