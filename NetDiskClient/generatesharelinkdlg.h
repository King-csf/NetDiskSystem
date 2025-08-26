#ifndef GENERATESHARELINKDLG_H
#define GENERATESHARELINKDLG_H

#include <QDialog>

namespace Ui {
class GenerateShareLinkDlg;
}

class GenerateShareLinkDlg : public QDialog
{
    Q_OBJECT

public:
    explicit GenerateShareLinkDlg(QWidget *parent = nullptr);
    ~GenerateShareLinkDlg();

    void setLink(QString shareLink);
    void setGetCode(QString code);

private:
    Ui::GenerateShareLinkDlg *ui;
};

#endif // GENERATESHARELINKDLG_H
