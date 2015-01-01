#include "recommendwidget.h"
#include <QSqlQuery>
#include <QMessageBox>
#include <QHostAddress>
#include <QNetworkInterface>
#include <QFileDialog>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
RecommendWidget::RecommendWidget(QWidget *parent) :
    QWidget(parent)
{
    hostName="www.mvgather.com";
    isFirstLoad = true;
    mainGLayout = new QGridLayout;

    mainGLayout->setMargin(0);
    setLayout(mainGLayout);

    listShowWidget = new ListShowWidget;
    dyRecommendBtn = new QPushButton;
    searchBtn = new QPushButton;
    searchLineEdit = new QLineEdit;

    searchBtn->setStyleSheet("QPushButton{background-color: rgba(0, 82, 112, 100);color: white;border: 1px;}QPushButton:hover{background-color: rgba(0, 82, 112, 200);color: white;border: 1px;}QPushButton:disabled{background-color: transparent;color: gray;}");

    mainGLayout->addWidget(listShowWidget,0,0,10,10);
    mainGLayout->addWidget(dyRecommendBtn,0,10,1,1);
    mainGLayout->addWidget(searchLineEdit,1,10,1,2);
    mainGLayout->addWidget(searchBtn,1,12,1,1);

    connect(listShowWidget,SIGNAL(clicked(QString,QString)),this,SLOT(addDownloadTask(QString,QString)));//this->tvUrl,this->title
    connect(listShowWidget,SIGNAL(pageChanged()),this,SLOT(loadData()));

    connect(dyRecommendBtn,SIGNAL(clicked()),this,SLOT(reLoadData()));
    dyRecommendBtn->setText("电影推荐");

    searchBtn->setText("搜索");
    connect(searchBtn,SIGNAL(clicked()),this,SLOT(search()));

    connect(searchLineEdit,SIGNAL(returnPressed()),this,SLOT(search()));
    searchLineEdit->setMaximumWidth(200);

    manager = new QNetworkAccessManager;
    QTimer::singleShot(5000,this,SLOT(counter()));
    //QTimer::singleShot(2000,this,SLOT(loadData()));

}

void RecommendWidget::loadDataFromServer(int page)
{
    myTipWin.show();
    myTipWin.setBusyIndicatorShow(true);
    myTipWin.setText("正在请求数据");
    QNetworkReply *reply = manager->get(QNetworkRequest(QUrl(QString("http://%1/mvgather/api/getMvList.php?page=%2").arg(hostName).arg(page))));
    QEventLoop loop;
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
    QByteArray data = reply->readAll();

    QStringList args;
    QString tvUrl;
    QString imgUrl;
    QString title;
    QString descripe;
    QStringList tempList = QString(data).split("$");

    tempList.removeAll("");
    foreach (QString alist, tempList) {
        tvUrl =alist.split("#").value(1,"");
        imgUrl =alist.split("#").value(2,"");
        title =alist.split("#").value(0,"");
        descripe =alist.split("#").value(3,"");
        args<<tvUrl+"#"+imgUrl+"#"+title+"#"+descripe;
    }
    if(args.isEmpty())
    {
//        QMessageBox msg;
//        msg.setText("没有找到数据。");
//        msg.exec();
        myTipWin.setText("没有找到数据。");
        myTipWin.timeToHide(3);
    }else
    {
        listShowWidget->loadDataToList(args);
        listShowWidget->showPage();
        myTipWin.setText("加载完毕");
        myTipWin.timeToHide(2);
    }

}

void RecommendWidget::loadData()
{
    int page= listShowWidget->page;
    loadDataFromServer(page);
}

void RecommendWidget::reLoadData()
{
    listShowWidget->page = 1;
    loadDataFromServer(1);
}

void RecommendWidget::addDownloadTask(QString url, QString title)//tvUrl,title
{
    emit addDownloadTaskSignal(url);

//    QString fileSavePath;
//    if(url.contains("plyz"))
//    {
//        QString fileName = url.split("=").last();
//        QString dirname = QFileDialog::getExistingDirectory(this, tr("选择保存目录:"), QDir::homePath());
//        if(dirname=="")
//        {
//            dirname = QDir::homePath();
//        }
//        fileSavePath=QString("%0/%1").arg(dirname).arg(fileName);

//        if(fileSavePath.isEmpty()) return;
//        fileSavePath = QFileInfo(fileSavePath).absoluteFilePath();
//    }
//    //qDebug()<<"file"<<fileSavePath;
//    downloadWidget = new Widget(0,url,fileSavePath);
//    connect(downloadWidget,SIGNAL(play(QString)),SLOT(play(QString)));
//    downloadWidget->show();

}

void RecommendWidget::search()
{
    if(searchLineEdit->text().trimmed()=="")
    {
        searchLineEdit->setText("在此填入文字!");
        searchLineEdit->selectAll();
        searchLineEdit->setFocus();
        return;
    }
    if(searchLineEdit->text().contains("*"))
    {
        searchLineEdit->setText("请不要搞破坏!");
        return;
    }
    if(searchLineEdit->text().contains(" "))
    {
        searchLineEdit->setText("请不要输入空格符!");
        return;
    }
    QString keyWord=searchLineEdit->text().trimmed();
    QNetworkReply *reply = manager->get(QNetworkRequest(QUrl(QString("http://%1/mvgather/api/search.php?keyWord=%2").arg(hostName).arg(keyWord))));
    QEventLoop loop;
    QTimer::singleShot(10000,&loop,SLOT(quit()));
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
    QByteArray data = reply->readAll();

    QStringList args;
    QString tvUrl;
    QString imgUrl;
    QString title;
    QString descripe;
    QStringList tempList = QString(data).split("$");

    tempList.removeAll("");
    foreach (QString alist, tempList) {
        tvUrl =alist.split("#").value(1,"");
        imgUrl =alist.split("#").value(2,"");
        title =alist.split("#").value(0,"");
        descripe =alist.split("#").value(3,"");
        args<<tvUrl+"#"+imgUrl+"#"+title+"#"+descripe;
    }
    if(args.isEmpty())
    {
//        QMessageBox msg;
//        msg.setText("没有找到数据。");
//        msg.exec();
        myTipWin.show();
        myTipWin.setText("没有找到数据。");
        myTipWin.timeToHide(3);
    }else
    {
        listShowWidget->loadDataToList(args);
    }

}

void RecommendWidget::counter()
{
    QList<QHostAddress> list = QNetworkInterface::allAddresses();
    QString ip;
    foreach (QHostAddress address, list)
    {
        if(address.protocol() == QAbstractSocket::IPv4Protocol)
            ip =address.toString();
    }
    if(ip=="")
    {
        ip="none";
    }
    QNetworkReply *reply =manager->get(QNetworkRequest(QUrl(QString("http://%1/mvgather/userHome/useCounter.php?ip=%2").arg(hostName).arg(ip))));
    QEventLoop loop;
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
}

