#include "loginform.h"
#include "ui_loginform.h"
#include <QDebug>
LoginForm::LoginForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginForm)
{
    setWindowFlags(Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint);

    ui->setupUi(this);
    ui->vCodeLbl->setVisible(false);
    ui->vCodelineEdit->setVisible(false);

    ui->lineEdit_2->setEchoMode(QLineEdit::Password);
    ui->lineEdit_2->setStyleSheet("lineedit-password-character: 42");

    isInitMark = false;

}

LoginForm::~LoginForm()
{
    delete ui;
}

void LoginForm::init(QNetworkAccessManager *manager)
{
    this->manager = manager;
    QSettings settings("MvGather", "xusongjie");
    QString str = settings.value("app/usrInfo", "").toString();
    ui->lineEdit->setText(str.split("#").value(0,""));
    ui->lineEdit_2->setText(str.split("#").value(1,""));

    QEventLoop loop;
    QNetworkReply *reply = manager->get(QNetworkRequest(QUrl("http://passport.baidu.com/v2/api/?getapi&tpl=mn&apiver=v3&class=login&logintype=dialogLogin")));
    QObject::connect(reply,SIGNAL(finished()), &loop, SLOT(quit()), Qt::DirectConnection);
    loop.exec();
    //QByteArray data = reply->readAll();

    QNetworkReply *reply1 = manager->get(QNetworkRequest(QUrl("http://passport.baidu.com/v2/api/?getapi&tpl=mn&apiver=v3&class=login&logintype=dialogLogin")));
    QObject::connect(reply1,SIGNAL(finished()), &loop, SLOT(quit()), Qt::DirectConnection);
    loop.exec();
    QByteArray data = reply1->readAll();

    QRegExp rx("token\" : \".*\"");
    rx.setMinimal(true);
    int pos = rx.indexIn(data);
    if (pos > -1) {
        QString tempStr = rx.cap(0);
        this->token = tempStr.replace(QRegExp("token\" : \"|\""),"");
        isInitMark = true;
    }else
    {
        myTipWin.show();
        myTipWin.setBusyIndicatorShow(false);
        myTipWin.setText("获取token失败");
        myTipWin.timeToHide(2);
        isInitMark = false;
    }
    //qDebug()<<this->token;
    on_lineEdit_editingFinished();

}

QString LoginForm::getToken()
{
    return token;
}

void LoginForm::on_lineEdit_editingFinished()
{

    QEventLoop loop;
    QNetworkReply *replyLoginCheck = manager->get(QNetworkRequest(QUrl(QString("http://passport.baidu.com/v2/api/?logincheck&token=%0&tpl=mm&apiver=v3&username=%1&isphone=false").arg(token).arg(ui->lineEdit->text()))));
    QObject::connect(replyLoginCheck,SIGNAL(finished()), &loop, SLOT(quit()), Qt::DirectConnection);
    loop.exec();
    QByteArray dataLoginCheck = replyLoginCheck->readAll();
    //qDebug()<<dataLoginCheck;

    QJsonParseError *error=new QJsonParseError;
    QJsonDocument doc=QJsonDocument::fromJson(dataLoginCheck,error);
    if(error->error==QJsonParseError::NoError)
    {
        QJsonObject rootObj = doc.object();
        QJsonObject dataObj = rootObj["data"].toObject();
        this->codestring = dataObj["codeString"].toString();
        this->vcodetype = dataObj["vcodetype"].toString();
    }
    if(!this->codestring.isEmpty())
    {
        QNetworkReply *replyVerifycode = manager->get(QNetworkRequest(QUrl(QString("http://passport.baidu.com/cgi-bin/genimage?%0").arg(codestring))));
        QObject::connect(replyVerifycode,SIGNAL(finished()), &loop, SLOT(quit()), Qt::DirectConnection);
        loop.exec();
        QByteArray dataVerifycode = replyVerifycode->readAll();
        QPixmap pixmap;
        pixmap.loadFromData(dataVerifycode);
        ui->vCodeLbl->setPixmap(pixmap);
        ui->vCodeLbl->setVisible(true);
        ui->vCodelineEdit->setVisible(true);

    }else
    {
        ui->vCodeLbl->setVisible(false);
        ui->vCodelineEdit->setVisible(false);
    }
}

void LoginForm::on_toolButton_clicked()
{
    myTipWin.show();
    myTipWin.setBusyIndicatorShow(true);
    myTipWin.setText("正在登录");
    ui->toolButton->setEnabled(false);
    QEventLoop loop;
    QByteArray postData;
    postData.append("staticpage=http://www.baidu.com/cache/user/html/v3Jump.html&");
    postData.append("charset=utf-8&");

    postData.append(QString("token=%0&").arg(token));
    postData.append("isPhone=false&");
    postData.append("quick_user=0&");
    postData.append("u=http://www.baidu.com/&");
    postData.append("safeflg=0&");
    postData.append("loginType=dialogLogin&");
    postData.append("logLoginType=pc_loginDialog&");
    postData.append("tpl=mn&");
    postData.append("loginmerge=true&");
    postData.append("splogin=rate&");

    postData.append(QString("username=%0&").arg(ui->lineEdit->text()));
    postData.append(QString("password=%0&").arg(ui->lineEdit_2->text()));

    postData.append(QString("codestring=%0&").arg(codestring));
    postData.append(QString("verifycode=%0&").arg(ui->vCodelineEdit->text()));
    postData.append("mem_pass=on");

    QNetworkRequest re(QUrl("http://passport.baidu.com/v2/api/?login"));
    re.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");
    QNetworkReply *reply2 = manager->post(re,postData);
    QTimer::singleShot(5000,&loop,SLOT(quit()));
    QObject::connect(reply2,SIGNAL(finished()), &loop, SLOT(quit()), Qt::DirectConnection);
    loop.exec();
    QByteArray data = reply2->readAll();
    //qDebug()<<data;
    if(data.contains("error=0"))//登录成功
    {
        myTipWin.setText("登录成功");

        QSettings settings("MvGather", "xusongjie");
        QString str;
        str.append(ui->lineEdit->text());
        str.append("#");
        str.append(ui->lineEdit_2->text());
        settings.setValue("app/usrInfo", str);

        QEventLoop loop;
        QNetworkReply *replyDiskInfo = manager->get(QNetworkRequest(QUrl(QString("http://pan.baidu.com/api/quota?channel=chunlei&clienttype=0&web=1"))));
        QTimer::singleShot(5000,&loop,SLOT(quit()));
        QObject::connect(replyDiskInfo,SIGNAL(finished()), &loop, SLOT(quit()), Qt::DirectConnection);
        loop.exec();
        QByteArray dataDiskInfo = replyDiskInfo->readAll();
    //qDebug()<<dataDiskInfo;
        QJsonParseError *error=new QJsonParseError;
        QJsonDocument doc=QJsonDocument::fromJson(dataDiskInfo,error);
        if(error->error==QJsonParseError::NoError)
        {
            QJsonObject rootObj = doc.object();
            QVariantMap rootMap = rootObj.toVariantMap();
            QString errnoV = rootMap.value("errno").toString();
            //qDebug()<<rootObj["errno"].toString();;
            if(errnoV == "0")
            {
                double used = rootMap.value("used").toDouble();
                double total = rootMap.value("total").toDouble();
                this->hide();
                emit panInfoChanger(QString("%0 GB/%1 GB").arg(QString::number(used/(1024*1024*1024))).arg(QString::number(total/(1024*1024*1024))));
                emit loginSuccess(ui->lineEdit->text(),this->token);
            }

        }
    }else
    {
        on_lineEdit_editingFinished();
        myTipWin.setText("登录失败");
    }
    myTipWin.timeToHide(2);

    ui->toolButton->setEnabled(true);

}
