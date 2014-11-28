#include "analyzer.h"
#include <QRegExp>
#include <QDebug>
#include <QVariant>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
Analyzer::Analyzer(QObject *parent) :
    QObject(parent)
{
}

QString Analyzer::funTvAnalyze(QString url)//http://www.sufeinet.com/thread-8196-1-1.html
{
    //qDebug()<<url;
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QString realUrl;
    QString mediaid;
    QString serial;
    QStringList urlParts = url.split("/");
    QString mediaidPart;
    foreach (QString part, urlParts) {
        if(part.contains("m-"))
        {
            mediaidPart = part;
            break;
        }
    }
    //qDebug()<<mediaidPart;
    QRegExp rx_mediaid("m-\\d{1,10}");//
    QRegExp rx_serial("e-\\d{1,10}");//集数
    //rx_mediaid.setMinimal(true);
    //rx_serial.setMinimal(true);
    int pos_mediaid = rx_mediaid.indexIn(mediaidPart);
    if (pos_mediaid > -1) {
        mediaid = rx_mediaid.cap(0);
    }
    int pos_serial = rx_serial.indexIn(mediaidPart);
    if (pos_serial > -1) {
        serial = rx_serial.cap(0);
    }
    mediaid.replace("m-","");
    serial.replace("e-","");
    //qDebug()<<mediaid<<serial;

    QString hashid;
    QString apiUrl1 = QString("http://api.funshion.com/ajax/vod_panel/%1/w-1").arg(mediaid);
    QNetworkReply *reply = manager->get(QNetworkRequest(QUrl(apiUrl1)));
    QEventLoop loop;
    QTimer::singleShot(8000,&loop,SLOT(quit()));//
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
    QByteArray data = reply->readAll();
    QRegExp rx_hashid("hashid\":\".*\"");
    rx_hashid.setMinimal(true);
    int pos_hashid = rx_hashid.indexIn(data);
    if (pos_hashid > -1) {
        hashid = rx_hashid.cap(0);
    }
    hashid.replace(QRegExp("hashid\":\"|\""),"");
    //qDebug()<<hashid;

    if(serial == "")
    {
        serial = "1";
    }
    //get token
    QString apiUrl2 = QString("http://api.funshion.com/ajax/get_webplayinfo/%1/%2/mp4?user=funshion").arg(mediaid).arg(serial);
    QNetworkReply *reply2 = manager->get(QNetworkRequest(QUrl(apiUrl2)));
    QTimer::singleShot(8000,&loop,SLOT(quit()));//
    QObject::connect(reply2, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
    data = reply2->readAll();

    QString token;
    QJsonParseError *error=new QJsonParseError;
    QJsonDocument doc =QJsonDocument::fromJson(data,error);
    if(error->error==QJsonParseError::NoError)
    {
        QJsonObject rootObj = doc.object();
        token = rootObj["token"].toString();
    }

    //get realUrl
    QString apiUrl3 = QString("http://jobsfe.funshion.com/query/v1/mp4/%1.json?clifz=fun&mac=&tm=1395365896&token=%2").arg(hashid).arg(token);
    QNetworkReply *reply3 = manager->get(QNetworkRequest(QUrl(apiUrl3)));
    QTimer::singleShot(8000,&loop,SLOT(quit()));//
    QObject::connect(reply3, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
    data = reply3->readAll();

    QJsonDocument realUrlDoc =QJsonDocument::fromJson(data,error);
    if(error->error==QJsonParseError::NoError)
    {
        QJsonObject rootObj = realUrlDoc.object();
        QJsonArray playlistArray = rootObj["playlist"].toArray();
        QJsonObject obj = playlistArray.first().toObject();
        realUrl = obj["urls"].toArray().first().toString();
    }


    return realUrl;
}
