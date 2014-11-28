#ifndef MYINPUTDIALOGMAINWIN_H
#define MYINPUTDIALOGMAINWIN_H
#include "QFramer/fbasedialog.h"
#include "myinputdialog.h"
class MyInputDialogMainWin : public FBaseDialog
{
    Q_OBJECT
public:
    explicit MyInputDialogMainWin(QWidget *parent = 0);
    MyInputDialog *centerWidget;
signals:
public slots:
    void urlInputInit(QString defaultUrl);
    QString getText();

};

#endif // MYINPUTDIALOGMAINWIN_H
