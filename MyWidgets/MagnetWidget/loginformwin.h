#ifndef LOGINFORMWIN_H
#define LOGINFORMWIN_H
#include "loginform.h"
#include "QFramer/fbasedialog.h"
#include <QLayout>
class LoginFormWin : public FBaseDialog
{
    Q_OBJECT
public:
    explicit LoginFormWin(QWidget *parent = 0);
    LoginForm *loginForm;
public slots:
private:


};

#endif // LOGINFORMWIN_H
