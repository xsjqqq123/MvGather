#ifndef LOGINFORM_H
#define LOGINFORM_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QEventLoop>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonParseError>
#include <QRegExp>
#include <QTimer>
#include <QSettings>
#include "../mytip.h"
namespace Ui {
class LoginForm;
}

class LoginForm : public QWidget
{
    Q_OBJECT

public:
    explicit LoginForm(QWidget *parent = 0);
    ~LoginForm();
    void init(QNetworkAccessManager *manager = NULL);
    bool isInitMark;
signals:
    void panInfoChanger(QString text);
    void loginSuccess(QString userName,QString token);
public slots:
    QString getToken();
private slots:
    void on_lineEdit_editingFinished();

    void on_toolButton_clicked();

private:
    Ui::LoginForm *ui;
    QString token;
    QNetworkAccessManager *manager;
    QString codestring;
    QString vcodetype;
    QString verifycode;
    MyTip myTipWin;

};

#endif // LOGINFORM_H
