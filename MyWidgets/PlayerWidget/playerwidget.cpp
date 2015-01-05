#include "playerwidget.h"
#include <QDateTime>
#include <QDebug>
#include <QVariant>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QCursor>
#include <QPoint>
#include <QMessageBox>
#include <QMenu>
#include <QAction>
#include <QDataStream>
#include <QTextCodec>
#include <QSettings>
#include <QClipboard>
#include <QApplication>
PlayerWidget::PlayerWidget(QWidget *parent) :
    QWidget(parent)
{
    currentSlice= 1;
    totalSlices = 1;
    totalSeries = 1;
    initUI();
    timerToUpdateAllHrefs = new QTimer;
    connect(timerToUpdateAllHrefs,SIGNAL(timeout()),this,SLOT(UpdateAllHrefs()));
    //timerToUpdateAllHrefs->start(10*60*1000);

    myTipWin = new MyTip(this);
    myTipWin->setBusyIndicatorShow(true);
    clipBoardWatcherTimer = new QTimer;
    connect(clipBoardWatcherTimer,SIGNAL(timeout()),this,SLOT(checkClipboard()));
    //clipBoardWatcherTimer->start(1000);

    flvxzTokens = "75ea44a540257938ea5683d3318643d8";
    QTimer::singleShot(1000,this,SLOT(getFlvxzTokens()));
}

void PlayerWidget::initUI()
{
    mainGLayout = new QGridLayout;
    mainGLayout->setMargin(0);
    mainGLayout->setSpacing(0);
    setLayout(mainGLayout);
    playerWin = new PlayerWin;
    progressBar =  new ProgressBar;
    listArea = new  PlayerListArea(this);
    tvListWidget = new TvListWidget(this);

    PlayerListAreaTab =  new QTabWidget(this);
    PlayerListAreaTab->tabBar()->setExpanding(true);
    PlayerListAreaTab->setMaximumWidth(240);
    PlayerListAreaTab->setMinimumWidth(210);
    PlayerListAreaTab->addTab(listArea,"播放列表");
    PlayerListAreaTab->addTab(tvListWidget,"电视直播");

//    PlayerListAreaTab->setObjectName("PlayerListAreaTab");
//    PlayerListAreaTab->setStyleSheet("#PlayerListAreaTab{background-color: rgb(35, 35, 35);}");

    connect(playerWin,SIGNAL(hideToFullScreen(bool)),PlayerListAreaTab,SLOT(setVisible(bool)));
    connect(playerWin,SIGNAL(hideToFullScreen(bool)),this,SLOT(SendHideToFullScreen(bool)));
    connect(playerWin,SIGNAL(getIntoWideModel(bool)),PlayerListAreaTab,SLOT(setVisible(bool)));
    connect(playerWin,SIGNAL(getIntoWideModel(bool)),this,SLOT(SendGetIntoWideModel(bool)));
    connect(playerWin,SIGNAL(percentChange(qreal)),progressBar,SLOT(setPercentPos(qreal)));
    connect(playerWin,SIGNAL(playFinished()),this,SLOT(handlePlayFinished()));
    connect(playerWin,SIGNAL(loadingMedia()),this,SLOT(handleLoadingMedia()));
    connect(playerWin,SIGNAL(progressBarDrawTxt(QString)),progressBar,SLOT(drawTimeText(QString)));
    connect(playerWin,SIGNAL(sendloadSlice(int)),this,SLOT(loadSlice(int)));
    connect(playerWin,SIGNAL(callForPlayer2Ready()),this,SLOT(makePlayer2Ready()));
    connect(playerWin,SIGNAL(resetSig()),this,SLOT(reset()));
    connect(listArea,SIGNAL(play(QString,QString)),this,SLOT(play(QString,QString)));
    connect(listArea,SIGNAL(syncMv()),this,SLOT(syncMv()));
    connect(tvListWidget,SIGNAL(play(QString,QString)),this,SLOT(addMplayListAndPlay(QString,QString)));
    connect(progressBar,SIGNAL(posChange(qreal)),this,SLOT(seekPos(qreal)));
    mainGLayout->addWidget(playerWin,0,0,1,1);
    mainGLayout->addWidget(progressBar,1,0,1,1);
    mainGLayout->addWidget(PlayerListAreaTab,0,1,2,1);
}

void PlayerWidget::addTvList(QString TvName, QStringList playno_hrefs)
{
    //操纵数据库，写入收藏，并发送让list更新信号
    //playlistTB(tvId VARCHAR( 30 ) NOT NULL,tvName VARCHAR( 30 ),tvno_hrefs VARCHAR( 30 ),historyNo VARCHAR( 30 ))");
    qint64 tvId = QDateTime::currentMSecsSinceEpoch();
    QString tvno_hrefs = playno_hrefs.join("$");
    QSqlQuery query_insert;
    query_insert.exec(QString("INSERT INTO playlistTB VALUES('%1','%2','%3','%4','','')").arg(tvId).arg(TvName).arg(tvno_hrefs).arg("0"));

    listArea->updateList();
}

void PlayerWidget::play(QString tvId, QString tvNO)
{

    //QPoint beforeP = QCursor::pos();
    //查找数据库，找到对应集数的href
    playingTvId = tvId;
    playingTvNO = tvNO.toInt();
    QSqlQuery query_select(QString("SELECT * FROM playlistTB WHERE tvId='%1'").arg(tvId));
    query_select.exec();
    QString href;
    while(query_select.next())
    {
        QString tvNo_hrefs = query_select.value("tvNo_hrefs").toString();
        QStringList tvNo_hrefList = tvNo_hrefs.split("$");
        tvNo_hrefList.removeAll("");
        totalSeries = tvNo_hrefList.count();
        foreach(QString tvNo_href,tvNo_hrefList)
        {
            QString tvNo_temp= tvNo_href.split("#").first();
            if(tvNO == tvNo_temp)
            {
                href = tvNo_href.split("#").last();
                playingHref = href;
                break;
            }
        }

    }
    getRealHref(href);

}

void PlayerWidget::getRealHref(QString href)
{
    qDebug()<<href;
    myTipWin->show();
    if(href == "")
    {
        myTipWin->setText("未设置地址href!");
        myTipWin->timeToHide(3);
        return;
    }
    myTipWin->setBusyIndicatorShow(true);
    myTipWin->setText("正在解析");
    allHrefs = "";
    //得到该地址的真实视频地址,并添加到player的list
    QNetworkAccessManager *manager = new QNetworkAccessManager;
    QRegExp rx(".*hunantv.com.*|.*wasu.cn.*|.*letv.com.*|.*iqiyi.com.*");//.*m1905.com.*|.*sohu.com.*|.*youku.com.*|.*tudou.com.*|.*kankan.com.*|.*pptv.com.*|.*fun.tv.*|.*pps.tv.*|.*qq.com.*|
    rx.setMinimal(true);
    QRegExp rx2(".*fun.tv.*|.*funshion.com.*");//.*fun.tv.*|.*funshion.com.*
    rx2.setMinimal(true);
    if(href.contains(rx2))
    {
        Analyzer analyser;
        QString files = analyser.funTvAnalyze(href);
        allHrefs = files;
        currentSlice =1;
        totalSlices = 1;
        playerWin->setTotalSlices(totalSlices);
        myTipWin->setText("播放视频。");
        addMplayListAndPlay(allHrefs,"高清");

    }else
    {
        qDebug()<<"anylise by flvxz.com";

        //得到网页源码
        QString hrefTemp = href;
        hrefTemp = QByteArray(hrefTemp.replace("://",":##").toUtf8()).toBase64();
        QNetworkRequest request(QUrl(QString("http://q1.flvxz.com/getFlv.php?url=%0&_=%1").arg(hrefTemp).arg(QDateTime::currentMSecsSinceEpoch())));
        request.setRawHeader(QByteArray("referer"), QByteArray("http://flv.cn/"));
        QEventLoop loop1;
        QNetworkReply *reply1 = manager->get(request);
        QObject::connect(reply1,SIGNAL(finished()), &loop1, SLOT(quit()), Qt::DirectConnection);
        loop1.exec();
        QString data1 = reply1->readAll();
        //qDebug()<<data1;
        //从源码中得到链接
        QRegExp rh("q1.flvxz.com.*\"");
        QRegExp rx_verify("/verify.*\"");
        rh.setMinimal(true);
        rx_verify.setMinimal(true);
        rh.indexIn(data1);
        QString url = rh.cap(0);
        rx_verify.indexIn(url);
        QString verify = rx_verify.cap(0).replace("\"","");

        url =QString("http://q1.flvxz.com/api/url/%0/jsonp/prettyjson%1").arg(hrefTemp).arg(verify);
        //qDebug()<<url;
        QNetworkReply *reply = manager->get(QNetworkRequest(QUrl(url)));
        QEventLoop loop;
        QTimer::singleShot(8000,&loop,SLOT(quit()));//
        QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
        loop.exec();
        QByteArray data = reply->readAll();
        //qDebug()<<data;

        QJsonParseError *error=new QJsonParseError;
        QJsonDocument doc=QJsonDocument::fromJson(data,error);

        QSettings settings("MvGather", "xusongjie");
        QString preferQualitysSetting=settings.value("app/preferQualitys", "").toString();
        QStringList preferQualitysList = preferQualitysSetting.split("#");

        if(error->error==QJsonParseError::NoError)
        {
            if(doc.isArray())
            {
                QJsonArray array=doc.array();
                bool breakMark = false;
                QString quality_temp;
                foreach (QString preferQuality, preferQualitysList) {

                    if(preferQuality.length()<5)
                    {
                        continue;
                    }
                    foreach (QJsonValue v, array) {
                        QJsonObject serial_obj = v.toObject();
                        quality_temp =serial_obj["quality"].toString();

//                        qDebug()<<preferQuality<<quality_temp;
                        if(quality_temp.contains(preferQuality))
                        {
                            breakMark = true;//得到与设置视频质量相符的地址

                            QJsonArray filesArray =serial_obj["files"].toArray();
                            QString files;
                            foreach (QJsonValue file, filesArray) {
                                QJsonObject file_obj = file.toObject();
                                files.append(file_obj["furl"].toString().replace("\\","")+"#");
                            }
                            allHrefs = files;
                            currentSlice =1;
                            totalSlices = filesArray.count();
                            playerWin->setTotalSlices(totalSlices);

                            break;
                        }
                        if(quality_temp == "")//wasu.cn
                        {
                            breakMark = true;
                            QJsonArray filesArray =serial_obj["files"].toArray();
                            QString files;
                            foreach (QJsonValue file, filesArray) {
                                QJsonObject file_obj = file.toObject();
                                files.append(file_obj["furl"].toString().replace("\\","")+"#");
                            }
                            allHrefs = files;
                            currentSlice =1;
                            totalSlices = filesArray.count();
                            playerWin->setTotalSlices(totalSlices);

                            break;
                        }
                    }
                    if(breakMark)
                    {
                        break;
                    }
                }
                myTipWin->setText("播放视频。");
                addMplayListAndPlay(allHrefs,quality_temp);

            }

        }else
        {
            myTipWin->setBusyIndicatorShow(false);
            myTipWin->setText("不支持解析!");
        }


    }
    if(allHrefs.isEmpty())
    {
        qDebug()<<"anylise by api.flvxz.com";
        int tokenCount = flvxzTokens.split("#").count();
        int n = 0;
        if(tokenCount>1)
        {
            n = qrand()%(tokenCount-1);
        }
        //qDebug()<<tokenCount<<n<<flvxzTokens.split("#").value(n,"75ea44a540257938ea5683d3318643d8");
        QString token = flvxzTokens.split("#").value(n,"75ea44a540257938ea5683d3318643d8");
        QString hrefTemp = href;
        QString url =QString("http://api.flvxz.com/token/%0/url/%1/jsonp/prettyjson").arg(token).arg(QString(QByteArray(hrefTemp.replace("://",":##").toUtf8()).toBase64()));
        //qDebug()<<url;
        QNetworkReply *reply = manager->get(QNetworkRequest(QUrl(url)));
        QEventLoop loop;
        QTimer::singleShot(8000,&loop,SLOT(quit()));//
        QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
        loop.exec();
        QByteArray data = reply->readAll();
        //qDebug()<<data;

        QJsonParseError *error=new QJsonParseError;
        QJsonDocument doc=QJsonDocument::fromJson(data,error);

        QSettings settings("MvGather", "xusongjie");
        QString preferQualitysSetting=settings.value("app/preferQualitys", "").toString();
        QStringList preferQualitysList = preferQualitysSetting.split("#");

        if(error->error==QJsonParseError::NoError)
        {
            if(doc.isArray())
            {
                QJsonArray array=doc.array();
                bool breakMark = false;
                QString quality_temp;
                foreach (QString preferQuality, preferQualitysList) {

                    if(preferQuality.length()<5)
                    {
                        continue;
                    }
                    foreach (QJsonValue v, array) {
                        QJsonObject serial_obj = v.toObject();
                        quality_temp =serial_obj["quality"].toString();

                        //qDebug()<<preferQuality<<quality_temp;
                        if(quality_temp.contains(preferQuality))
                        {
                            breakMark = true;//得到与设置视频质量相符的地址

                            QJsonArray filesArray =serial_obj["files"].toArray();
                            QString files;
                            foreach (QJsonValue file, filesArray) {
                                QJsonObject file_obj = file.toObject();
                                files.append(file_obj["furl"].toString().replace("\\","")+"#");
                            }
                            allHrefs = files;
                            currentSlice =1;
                            totalSlices = filesArray.count();
                            playerWin->setTotalSlices(totalSlices);

                            break;
                        }
                        if(quality_temp == "")//wasu.cn
                        {
                            breakMark = true;
                            QJsonArray filesArray =serial_obj["files"].toArray();
                            QString files;
                            foreach (QJsonValue file, filesArray) {
                                QJsonObject file_obj = file.toObject();
                                files.append(file_obj["furl"].toString().replace("\\","")+"#");
                            }
                            allHrefs = files;
                            currentSlice =1;
                            totalSlices = filesArray.count();
                            playerWin->setTotalSlices(totalSlices);

                            break;
                        }
                    }
                    if(breakMark)
                    {
                        break;
                    }
                }
                myTipWin->setText("播放视频。");
                addMplayListAndPlay(allHrefs,quality_temp);

            }

        }else
        {
            myTipWin->setBusyIndicatorShow(false);
            myTipWin->setText("不支持解析!");
        }


    }
    if(allHrefs.isEmpty())
    {
        qDebug()<<"anylise by flvcd.com";
        QString format;
        int index=-1;
        QSettings settings("MvGather", "xusongjie");
        QString preferQualitysSetting=settings.value("app/preferQualitys", "").toString();
        QStringList qualityList;
        qualityList<<"M3U8"<<"高清"<<"超清";
        QStringList listTemp=preferQualitysSetting.split("#");
        foreach (QString quality, listTemp) {
            if(qualityList.contains(quality))
            {
                index=qualityList.indexOf(quality);
                break;
            }
        }
        if(index==-1)
        {
            format="high";
        }else
        {
            if(qualityList.value(index,"高清")=="M3U8")
            {
                format="normal";
            }else if(qualityList.value(index,"高清")=="高清")
            {
                format="high";
            }else
            {
                format="super";
            }
        }

        QString url =QString("http://www.flvcd.com/parse.php?kw=%1&flag=one&format=%2").arg(href).arg(format);
        QNetworkReply *reply = manager->get(QNetworkRequest(QUrl(url)));
        QEventLoop loop;
        QTimer::singleShot(8000,&loop,SLOT(quit()));//
        QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
        loop.exec();
        QByteArray r = reply->readAll();
        QTextCodec *gbk = QTextCodec::codecForName("GB2312");
        QString data = gbk->toUnicode(r);
        //qDebug()<<data;

        //得到下载列表字符串
        QRegExp rx_fileUrls("下载地址：.*</td>");
        rx_fileUrls.setMinimal(true);
        int pos=0;
        QString fileUrlsStr;
        while ((pos = rx_fileUrls.indexIn(data, pos)) != -1) {
            fileUrlsStr.append(rx_fileUrls.cap(0));
            pos += rx_fileUrls.matchedLength();
        }
        //提取出所有href
        QRegExp rx_file("href=\".*\"");
        rx_file.setMinimal(true);
        pos=0;
        while ((pos = rx_file.indexIn(fileUrlsStr, pos)) != -1) {
            QString tempStr = rx_file.cap(0);
            allHrefs.append(tempStr.replace(QRegExp("href=\"|\""),"")+"#");
            pos += rx_file.matchedLength();
        }
        currentSlice =1;
        QStringList aList = allHrefs.split("#");
        aList.removeAll("");
        totalSlices = aList.count();
        playerWin->setTotalSlices(totalSlices);
        myTipWin->setText("播放视频。");
        addMplayListAndPlay(allHrefs,format);
    }
    qDebug()<<"所有直链（以#分隔）："<<allHrefs;
    myTipWin->setText("解析结束。");
    delete manager;
    myTipWin->timeToHide(1);
}

void PlayerWidget::addMplayListAndPlay(QString filesUrl,QString quality)
{
    this->quality =  quality;
    QStringList fileList =  filesUrl.split("#");
    fileList.removeAll("");
    QString firstFile;
    if(fileList.count()>0)
    {
        firstFile =fileList.value(0);
        //this->playingHref = firstFile;
        fileList.removeFirst();
        playerWin->newPlay(firstFile);
        //        if(currentSlice==1)
        //        {
        //            playerWin->player->play(firstFile);
        //        }else
        //        {

        //        }

        playerWin->state=1;
    }
    //    if(fileList.count()>0)
    //    {
    //        fileList_temp.clear();
    //        fileList_temp.append(fileList);
    //        QTimer::singleShot(10000,this,SLOT(addFilesTimer()));

    //    }

}

void PlayerWidget::SendHideToFullScreen(bool b)
{
    emit hideToFullScreen(b);

}

void PlayerWidget::SendGetIntoWideModel(bool b)
{
    emit getIntoWideModel(b);
}

void PlayerWidget::updatePlayList()
{
    listArea->updateList();
}

void PlayerWidget::Player2Ready(int sliceIndex)
{
    readyHref = "";
    QString fileUrl = "";
    if(playingHref.contains("qiyi") && (currentSlice%2)==0)
    {
        qDebug()<<"iqiyi anylise;";
        myTipWin->show();
        myTipWin->setBusyIndicatorShow(true);
        myTipWin->setText("正在解析爱奇艺");

        qDebug()<<"anylise by flvxz.com";

        //得到网页源码

        QString hrefTemp = playingHref;
        hrefTemp = QByteArray(hrefTemp.replace("://",":##").toUtf8()).toBase64();
        QNetworkRequest request(QUrl(QString("http://q1.flvxz.com/getFlv.php?url=%0&_=%1").arg(hrefTemp).arg(QDateTime::currentMSecsSinceEpoch())));
        request.setRawHeader(QByteArray("referer"), QByteArray("http://flv.cn/"));
        QEventLoop loop1;
        QNetworkAccessManager *manager = new QNetworkAccessManager;
        QNetworkReply *reply1 = manager->get(request);
        QObject::connect(reply1,SIGNAL(finished()), &loop1, SLOT(quit()), Qt::DirectConnection);
        loop1.exec();
        QString data1 = reply1->readAll();
        //qDebug()<<data;
        //从源码中得到链接
        QRegExp rh("href=\".*\"");
        QRegExp rx_verify("/verify.*\"");
        rh.setMinimal(true);
        rx_verify.setMinimal(true);
        rh.indexIn(data1);
        QString url = rh.cap(0);
        rx_verify.indexIn(url);
        QString verify = rx_verify.cap(0).replace("\"","");

        url =QString("http://q1.flvxz.com/api/url/%0/jsonp/prettyjson%1").arg(hrefTemp).arg(verify);
        //qDebug()<<url;
        QNetworkReply *reply = manager->get(QNetworkRequest(QUrl(url)));
        QEventLoop loop;
        QTimer::singleShot(8000,&loop,SLOT(quit()));//
        QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
        loop.exec();
        QByteArray data = reply->readAll();
        //解析数据得到，对应片数的真实地址fileUrl
        QJsonParseError *error=new QJsonParseError;
        QJsonDocument doc=QJsonDocument::fromJson(data,error);
        if(error->error==QJsonParseError::NoError)
        {
            if(doc.isArray())
            {
                QJsonArray array=doc.array();
                QJsonObject obj1=array.first().toObject();
                QJsonArray filesArray =obj1["files"].toArray();
                QJsonObject fileObj =filesArray[sliceIndex].toObject();
                fileUrl = fileObj["furl"].toString().replace("\\","");
                if(fileUrl =="")
                {
                }else
                {
                    allHrefs = "";
                    foreach (QJsonValue v, filesArray) {//更新iqiyi所有片的下载地址
                        QJsonObject fileSliceObj = v.toObject();
                        allHrefs.append(QString("%0#").arg(fileSliceObj["furl"].toString()));
                    }
                    //qDebug()<<allHrefs;
                }
            }
        }
        delete manager;
    }else
    {
        fileUrl = allHrefs.split("#").value(sliceIndex,"");
    }
    //fileUrl = allHrefs.split("#").value(sliceIndex,"");
    if(fileUrl == "")
    {
        fileUrl = allHrefs.split("#").value(sliceIndex,"");
    }
    readyHref = fileUrl;
    myTipWin->timeToHide(3);
}

void PlayerWidget::makePlayer2Ready()
{
    Player2Ready(this->currentSlice);
}

void PlayerWidget::writeQuitPro()
{
    playerWin->player->stop();
}

void PlayerWidget::handlePlayFinished()
{
    //qDebug()<<"Slice:"<<currentSlice<<"/"<<totalSlices;
    if(currentSlice>=totalSlices)
    {
        progressBar->drawTimeText("已播放完！");
        playNextOne();
    }else
    {
        myTipWin->show();
        myTipWin->setBusyIndicatorShow(true);
        myTipWin->setText("正在加载");
        if(!playerWin->state ==0)
        {
            if(readyHref == "")
            {
                makePlayer2Ready();
            }
            currentSlice+=1;
            addMplayListAndPlay(readyHref,this->quality);

        }
        myTipWin->timeToHide(0.2);
    }
}

void PlayerWidget::handleLoadingMedia()
{
    progressBar->drawTimeText(QString("播放中……(%1/%2)").arg(currentSlice).arg(totalSlices));
}

void PlayerWidget::PlayNewUrls(QString urlsToPlay, QString quality)
{
    addMplayListAndPlay(urlsToPlay,quality);
}

void PlayerWidget::loadSlice(int i)
{
    myTipWin->show();
    myTipWin->setText("正在加载");
    if(i==-1)
    {
        if(currentSlice==1)
        {
            //QMessageBox::warning(this,"提醒:","当前已是第一片",QMessageBox::Ok);

            myTipWin->setBusyIndicatorShow(false);
            myTipWin->setText("当前已是第一片!");
            myTipWin->timeToHide(3);
            return;
        }else
        {
            currentSlice -=1;
            Player2Ready(currentSlice-1);

        }
    }else if(i==1)
    {
        if(currentSlice==totalSlices)
        {
            //            QMessageBox::warning(this,"提醒:","当前已是最后一片",QMessageBox::Ok);
            myTipWin->show();
            myTipWin->setBusyIndicatorShow(false);
            myTipWin->setText("当前已是最后一片!");
            myTipWin->timeToHide(3);
            return;
        }else
        {
            Player2Ready(currentSlice);
            currentSlice+=1;
        }
    }/*else
    {
        Player2Ready(currentSlice-1);
    }*/
    playerWin->player->stop();
    playerWin->player->play(readyHref);
    myTipWin->timeToHide(1);
}

void PlayerWidget::addFilesTimer()
{

}

void PlayerWidget::syncMv()
{
    myTipWin->show();
    myTipWin->setBusyIndicatorShow(true);
    myTipWin->setText("正在同步");
    QSqlQuery query_select;
    query_select.exec(QString("SELECT * FROM playlistTB"));
    //tvId:该视频唯一编号,tvName:视频中文名.tvno_hrefs:集数与相应地址...historyNo:上次观看到的集数,tvUrl:source url;source:视频来源标识
    while(query_select.next())
    {
        QString tvId = query_select.value("tvId").toString();
        QString tvUrl = query_select.value("tvUrl").toString();
        QString source = query_select.value("source").toString();
        if(source=="")
        {
            continue;
        }else if(source == "letv")
        {
            source = "leshi";
        }
        QNetworkAccessManager *manager = new QNetworkAccessManager;
        QNetworkReply *reply = manager->get(QNetworkRequest(QUrl(tvUrl)));
        QEventLoop loop;
        QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
        loop.exec();
        QByteArray data = reply->readAll();
        if(data=="")
        {
            continue;
        }
        anyliseData(data,tvId,tvUrl,source);
    }
    updatePlayList();
    //    QMessageBox msgBox;
    //    msgBox.setText("同步完成! +_+");
    //    msgBox.exec();
    myTipWin->setText("同步完成! +_+");
    myTipWin->timeToHide(3);


}

void PlayerWidget::anyliseData(QByteArray data, QString tvId, QString tvUrl, QString source)
{
    QString tvNoHrefListStr;//tvNoHrefListStr:1#href
    //find tvName(title) and QStringList serials//来源名称及剧集列表
    //    QRegExp rx_title("<span class=\"title\">.*</span>");
    //    rx_title.setMinimal(true);
    int pos = 0;
    //    QString title;
    //    while ((pos = rx_title.indexIn(data, pos)) != -1) {
    //        title=rx_title.cap(0);
    //        pos += rx_title.matchedLength();
    //    }
    //    this->tvName = title.replace(QRegExp("<span class=\"title\">|</span>"),"");

    if(data.contains("好看的综艺"))//综艺大分类
    {
        //取出地址中的影视ID,http://www.yunfan.com/show/va/6287162973540335925.html
        QString id = tvUrl.split("/").last().replace(".html","");
        //        QRegExp rx_vlist("\"tv_ico\".*</div>");
        //        QRegExp rx_vlist_one("source=\'.*\'");
        //        QStringList vlist;
        //        rx_vlist.setMinimal(true);
        //        rx_vlist_one.setMinimal(true);
        //        pos = 0;
        //        QString vlistStr;
        //        while ((pos = rx_vlist.indexIn(data, pos)) != -1) {
        //            vlistStr=rx_vlist.cap(0);
        //            pos += rx_vlist.matchedLength();
        //        }
        //        pos=0;
        //        while ((pos = rx_vlist_one.indexIn(vlistStr, pos)) != -1) {
        //            QString str=rx_vlist_one.cap(0);
        //            vlist.append(str.replace(QRegExp("source=\'|\'"),""));
        //            pos += rx_vlist_one.matchedLength();
        //        }

        //        foreach (QString v, vlist) {
        QString jsonUrl = QString("http://www.yunfan.com/show/vapage.php?id=%1&site=%2").arg(id).arg(source);
        QNetworkAccessManager *manager = new QNetworkAccessManager;
        QNetworkReply *reply = manager->get(QNetworkRequest(QUrl(jsonUrl)));
        QEventLoop loop;
        QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
        loop.exec();
        QByteArray data = reply->readAll();
        pos=0;
        QRegExp rx_href("href=\'.*\'");
        rx_href.setMinimal(true);
        QString href_temp;
        int i=1;
        while ((pos = rx_href.indexIn(data, pos)) != -1 ) {//(pos = rx_href.indexIn(data, pos)) != -1) &&
            href_temp=rx_href.cap(0);
            href_temp.replace(QRegExp("href=\'|\'"),"");
            href_temp.replace("\\","");
            tvNoHrefListStr.append(QString::number(i)+"#"+href_temp.split("#").value(0,"")+"$");
            pos += rx_href.matchedLength();
            i++;
            //            }



        }

        //        if(vlist.count()<1)
        //        {
        //            QLabel *lb= new QLabel("无法找到可播放的源，可能该视频收费!\n你可以试着搜索一下。");
        //            mainLayout->addWidget(lb,0,0,2,2);
        //        }else
        //        {
        //            QLabel *lb= new QLabel("选择一个播放源:\n(一般排前面的较好。)");
        //            mainLayout->addWidget(lb,0,0,2,2);
        //            int column=0;
        //            foreach (QString txt, vlist) {
        //                btn = new QPushButton;
        //                btnGroup->addButton(btn,column);
        //                btn->setText(txt);
        //                btn->setMinimumSize(100,40);
        //                mainLayout->addWidget(btn,2,column,1,1);
        //                column++;
        //            }
        //        }
    }else if(data.contains("tv_ico"))//电视剧或动漫
    {
        //以view-source:http://www.yunfan.com/show/tv/6026173063460419400.html为例，先取出所有<div class="tv_ico">与</div>间的source=xxx内容
        //        QRegExp rx_vlist("\"tv_ico\".*</div>");
        //        QRegExp rx_vlist_one("source=\'.*\'");
        //        QStringList vlist;
        //        rx_vlist.setMinimal(true);
        //        rx_vlist_one.setMinimal(true);
        //        pos = 0;
        //        QString vlistStr;
        //        while ((pos = rx_vlist.indexIn(data, pos)) != -1) {
        //            vlistStr=rx_vlist.cap(0);
        //            pos += rx_vlist.matchedLength();
        //        }

        //        pos=0;
        //        while ((pos = rx_vlist_one.indexIn(vlistStr, pos)) != -1) {
        //            QString str=rx_vlist_one.cap(0);
        //            vlist.append(str.replace(QRegExp("source=\'|\'"),""));
        //            pos += rx_vlist_one.matchedLength();
        //        }

        //        foreach (QString v, vlist) {
        QRegExp rx_serial(QString("source_%1.*source=\'%2\'.*</div>").arg(source).arg(source));
        rx_serial.setMinimal(true);
        pos=0;
        while ((pos = rx_serial.indexIn(data, pos)) != -1) {//得到某个来源的播放列表
            QString str=rx_serial.cap(0);
            pos += rx_serial.matchedLength();
            QStringList data = str.split("</li>");
            data.removeLast();
            //单条数据中的href

            foreach (QString a , data) {
                QString href;
                QString playno;
                QRegExp rx_t("href=\'.*\'");
                rx_t.setMinimal(true);
                int pos = rx_t.indexIn(a);
                if (pos > -1) {
                    href = rx_t.cap(0);
                    href.replace(QRegExp("href=\'|\'"),"");
                }
                QRegExp rx_temp(">\\d{1,5}</a>");
                rx_temp.setMinimal(true);
                pos = rx_temp.indexIn(a);
                if (pos > -1) {
                    playno = rx_temp.cap(0);
                    playno.replace(QRegExp(">|</a>"),"");
                }
                if(href==""||playno=="")
                {
                    continue;
                }
                tvNoHrefListStr.append(playno+"#"+href.split("#").value(0,"")+"$");

            }
        }
    }else//是电影，单集
    {
        QRegExp rx_lylist("dy.xq.lylist.*</div>");
        rx_lylist.setMinimal(true);
        int pos = 0;
        QString lylistStr;//取到单条的来源列表数据
        while ((pos = rx_lylist.indexIn(data, pos)) != -1) {
            lylistStr=rx_lylist.cap(0);
            pos += rx_lylist.matchedLength();
        }
        QStringList lylist = lylistStr.split("</a>");
        lylist.removeLast();//最后一条没有数据
        if(lylist.count()<1)
        {
        }else
        {
            foreach (QString a, lylist) {
                if(!a.contains(source))
                {
                    continue;
                }
                QString href;//视频网站播放地址
                QRegExp rx_temp("href=\'.*\'");
                rx_temp.setMinimal(true);
                pos = 0;
                while ((pos = rx_temp.indexIn(a, pos)) != -1) {
                    href=rx_temp.cap(0);
                    href.remove(QRegExp("href=\'|\'"));
                    pos += rx_temp.matchedLength();
                }
                tvNoHrefListStr.append("1#"+href.split("#").value(0,""));
            }


        }
    }
    //qDebug()<<"tvNoHrefListStr:"<<tvNoHrefListStr;
    QSqlQuery query_update;
    query_update.exec(QString("UPDATE playlistTB SET tvno_hrefs = '%1' WHERE tvId='%2'").arg(tvNoHrefListStr).arg(tvId));
}

void PlayerWidget::UpdateAllHrefs()
{
    if(!playingHref.contains("qiyi"))
    {
        return;
    }
    if(totalSlices > 1 && (playerWin->player->isPlaying() || playerWin->player->isPaused()))
    {
        QString playingHref_temp = playingHref;

        int tokenCount = flvxzTokens.split("#").count();
        int n = 0;
        if(tokenCount>1)
        {
            n = qrand()%(tokenCount-1);
        }
        //qDebug()<<tokenCount<<n<<flvxzTokens.split("#").value(n,"75ea44a540257938ea5683d3318643d8");
        QString token = flvxzTokens.split("#").value(n,"75ea44a540257938ea5683d3318643d8");

        QString url =QString("http://api.flvxz.com/token/%0/url/%1/jsonp/prettyjson").arg(token).arg(QString(QByteArray(playingHref_temp.replace("://",":##").toUtf8()).toBase64()));

        QNetworkAccessManager *manager = new QNetworkAccessManager;
        QNetworkReply *reply = manager->get(QNetworkRequest(QUrl(url)));
        QEventLoop loop;
        QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
        loop.exec();
        QByteArray data = reply->readAll();

        QJsonParseError *error=new QJsonParseError;
        QJsonDocument doc=QJsonDocument::fromJson(data,error);
        if(error->error==QJsonParseError::NoError)
        {
            if(doc.isArray())
            {
                QJsonArray array=doc.array();
                QString quality_temp;
                foreach (QJsonValue v, array) {
                    QJsonObject serial_obj = v.toObject();
                    quality_temp =serial_obj["quality"].toString();

                    if(quality_temp == this->quality)
                    {
                        QJsonArray filesArray =serial_obj["files"].toArray();
                        QString files;
                        foreach (QJsonValue file, filesArray) {
                            QJsonObject file_obj = file.toObject();
                            files.append(file_obj["furl"].toString().replace("\\","")+"#");
                        }
                        allHrefs = files;
                        //qDebug()<<"update allHrefs:"<<allHrefs;
                        break;
                    }
                }

            }

        }
    }

}

void PlayerWidget::getFlvxzTokens()
{
    QString url("http://www.mvgather.com/mvgather/api/getFlvxzToken.php");
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QNetworkReply *reply = manager->get(QNetworkRequest(QUrl(url)));
    QEventLoop loop;
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
    QByteArray data = reply->readAll();
    if(data.contains("<html>") || data == "")
    {

    }else
    {
        flvxzTokens = data;
    }
}

void PlayerWidget::checkClipboard()
{
    QClipboard *clipboard = QApplication::clipboard();
    QString url = clipboard->text();
    if(url == clipBoardHrefPre)
    {
        return;
    }else
    {
        clipBoardHrefPre = url;
    }
    QRegExp rx(".*sohu.com.*|.*youku.com.*|.*tudou.com.*|.*iqiyi.com.*|.*v.qq.com/.*|.*letv.com.*|.*fun.tv.*|.*m1905.com.*|.*pps.tv.*|.*pptv.com.*|.*hunantv.com.*|.*kankan.com.*");
    rx.setMinimal(true);
    if(url.contains(rx))
    {
        if(!url.startsWith("http://"))
        {
            url.prepend("http://");
        }
        getRealHref(url);
    }
}

void PlayerWidget::playNextOne()
{
    playingTvNO = playingTvNO+1;
    if(playingTvNO > totalSeries)
    {
        return;
    }
    QSqlQuery query_select(QString("update playlistTB set historyNo='%0' where tvId='%1'").arg(playingTvNO).arg(playingTvId));
    query_select.exec();
    updatePlayList();
    play(playingTvId,QString::number(playingTvNO));

}

void PlayerWidget::seekPos(qreal pos)
{
    playerWin->seekPos(pos);

}

void PlayerWidget::reset()
{
    this->totalSlices = 0;
    this->currentSlice = 0;
}
