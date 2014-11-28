#include "myinputdialogmainwin.h"

MyInputDialogMainWin::MyInputDialogMainWin(QWidget *parent) :
    FBaseDialog(parent)
{
    centerWidget = new MyInputDialog(this);
    this->fMainLayout->addWidget(centerWidget);

}

void MyInputDialogMainWin::urlInputInit(QString defaultUrl)
{
    getTitleBar()->getTitleLabel()->setText("请输入:");
    centerWidget->setLblText("直链Url:");
    centerWidget->setLineEditText(defaultUrl);
}

QString MyInputDialogMainWin::getText()
{
    return centerWidget->getText();
}
