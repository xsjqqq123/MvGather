#include "loginformwin.h"

LoginFormWin::LoginFormWin(QWidget *parent) :
    FBaseDialog(parent)
{
    loginForm = new LoginForm;
    connect(loginForm,SIGNAL(loginSuccess(QString,QString)),this,SLOT(hide()));
    this->normalSize = QSize(300,200);
    getTitleBar()->getTitleLabel()->setText("登录百度云");
    this->layout()->addWidget(loginForm);
}
