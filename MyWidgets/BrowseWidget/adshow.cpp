#include "adshow.h"
#include <QMouseEvent>
#include <QDesktopServices>
#include <QDebug>
AdShow::AdShow(QWidget *parent) :
    QWidget(parent)
{
    mainGird =  new QGridLayout(this);
    imgLbl = new QLabel;
    //imgLbl->setScaledContents(true);
    txtLbl = new QLabel;
    txtLbl->setOpenExternalLinks(true);
    txtLbl->setWordWrap(true);
    mainGird->addWidget(imgLbl,0,0,2,5);
    mainGird->addWidget(txtLbl,2,0,1,5);
    setLayout(mainGird);

    host = "http://www.mvgather.com";
    QTimer::singleShot(5000, this, SLOT(getData()));


}

void AdShow::getData()
{
    QString url;
    url.append(host);
    url.append("/mvgather/announcement/announcement.php");
    manager = new QNetworkAccessManager;
    QNetworkReply *reply = manager->get(QNetworkRequest(QUrl(url)));
    QEventLoop loop;
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
    QString data = QString(reply->readAll());
    //qDebug()<<data;
    /*
    分为文本公告与图片公告：
    文本公告格式为：text#公告文字。
    图片公告格式为：pic#图片地址#文字
*/
    if(data =="")
    {
        return;
    }
    QStringList list_temp = data.split("#");
    if(list_temp.value(0,"") == "text")
    {
        txtLbl->setText(list_temp.value(1,""));
    }else if(list_temp.value(0,"") == "pic")
    {
        showPicAd(list_temp.value(1,""),list_temp.value(2,""));
    }
}

void AdShow::showPicAd(QString picUrl, QString txt)
{
    txtLbl->setText(txt);
    QNetworkReply *reply1 = manager->get(QNetworkRequest(QUrl(picUrl)));
    QEventLoop loop1;
    QObject::connect(reply1, SIGNAL(finished()), &loop1, SLOT(quit()));
    loop1.exec();
    QByteArray data = reply1->readAll();
    if(data =="")
    {
        return;
    }
    QPixmap pix;
    pix.loadFromData(data);
    imgLbl->setPixmap(pix);
}
