#include "centerwindow.h"
#include <QDir>
#include <QSqlQuery>
#include <QDebug>
#include <QMenu>
#include <QCursor>
#include <QSettings>
#include <QContextMenuEvent>
#include <QTimer>//
#include <QDateTime>
#include <QMessageBox>
#include <QDesktopServices>
#include <QDesktopWidget>
#include <QApplication>
#include <QDesktopWidget>
#include <QClipboard>
#include <QApplication>
#include <QFileDialog>
#include <QCursor>

CenterWindow::CenterWindow(QWidget *parent) :
    FCenterWindow(parent)
{
    this->version = "3.2.0";
    QDir dir;
    QDir dir2(dir.homePath()+"/视频");
    QDir dir3(dir.homePath()+"/Videos");
    QString dbPath;
    if(dir2.exists())
    {
        dbPath = dir.homePath()+"/视频/MvGather/Database";
    }else if(dir3.exists())
    {
        dbPath = dir.homePath()+"/Videos/MvGather/Database";
    }else
    {
        dbPath = dir.homePath()+"/MvGather";
    }
    dir.mkpath(dbPath);
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");//添加数据库驱动，这里用sqlite
    db.setDatabaseName(dbPath+"/MvGather.db");
    //    db.setDatabaseName("MvGather.db");
    if(db.open())
    {
        //tvId:该视频唯一编号,tvName:视频中文名.tvno_hrefs:集数与相应地址...historyNo:上次观看到的集数;quality:清晰度;tvUrl:yunfan视频列表地址;source:视频来源标识
        QSqlQuery query_creat_tb("CREATE TABLE IF NOT EXISTS playlistTB(tvId VARCHAR( 30 ) NOT NULL,tvName VARCHAR( 30 ),tvno_hrefs VARCHAR(100),historyNo VARCHAR( 30 ),quality VARCHAR( 30 ),tvUrl VARCHAR(100),source VARCHAR( 30 ))");
        query_creat_tb.exec();
        //taskId:创建下载任务的id;url任务原地址;fileSavePath:文件保存目录,percent完成的百分比
        QSqlQuery query_creat_tb2("CREATE TABLE IF NOT EXISTS dtaskTB(taskId VARCHAR(30) NOT NULL,url VARCHAR(200) NOT NULL,fileSavePath VARCHAR(200) NOT NULL,percent VARCHAR(5))");
        query_creat_tb2.exec();
    }


    playerWidget = new PlayerWidget(this);
    addWidget(tr(""), tr("Player"), playerWidget);//播放器

    browseWidget = new  BrowseWidget(this);
    addWidget(tr(""), tr("MvList"), browseWidget);//视频库

    recommendWidget = new RecommendWidget(this);
    addWidget(tr(""), tr("MvRecomend"), recommendWidget);//推荐

    magnetWidget = new MagnetWidget(this);
    addWidget(tr(""), tr("Magnet"), magnetWidget);//磁力链

    downloadManageWidget = new QScrollArea(this);
    addWidget(tr(""), tr("Download"), downloadManageWidget);//下载

    downloadManageScrollAreaWidget = new QWidget(downloadManageWidget);
    downloadManageWidget->setWidget(downloadManageScrollAreaWidget);
    downloadManageScrollAreaWidgetMainLayout = new QVBoxLayout;
    downloadManageScrollAreaWidgetMainLayout->setAlignment(Qt::AlignTop);
    downloadManageScrollAreaWidget->setLayout(downloadManageScrollAreaWidgetMainLayout);

    downloadManageScrollAreaWidget->setStyleSheet("background:transparent");

    getNavgationBar()->setCurrentIndex(0);
    setAlignment(LeftTop);

    QSettings settings("MvGather", "xusongjie");
    QString preferQualitysSetting = settings.value("app/preferQualitys", "").toString();
    if(preferQualitysSetting =="")
    {
        preferQualitysSetting="高清#超清#M3U8#分段_高清_FLV#分段_高清_MP4#分段_高清_M3U8#分段_720P_FLV#分段_720P_MP4#分段_720P_M3U8#分段_1080P_FLV#分段_1080P_MP4#分段_1080P_M3U8#分段_超清_FLV#分段_超清_MP4#分段_超清_M3U8#分段_标清_FLV#分段_标清_MP4#分段_标清_M3U8#分段_高码1080P_FLV#分段_高码1080P_MP4#分段_高码1080P_M3U8#分段_原画_FLV#分段_原画_MP4#分段_原画_M3U8#分段_4K_FLV#分段_4K_MP4#分段_4K_M3U8#分段_高码4K_FLV#分段_高码4K_MP4#分段_高码4K_M3U8#分段_低清_FLV#分段_低清_MP4#分段_低清_M3U8#单段_高清_MP4#单段_高清_M3U8#单段_高清_FLV#单段_720P_FLV#单段_720P_MP4#单段_720P_M3U8#单段_1080P_FLV#单段_1080P_MP4#单段_1080P_M3U8#单段_超清_FLV#单段_超清_MP4#单段_超清_M3U8#单段_标清_FLV#单段_标清_MP4#单段_标清_M3U8#单段_高码1080P_FLV#单段_高码1080P_MP4#单段_高码1080P_M3U8#单段_原画_FLV#单段_原画_MP4#单段_原画_M3U8#单段_4K_FLV#单段_4K_MP4#单段_4K_M3U8#单段_高码4K_FLV#单段_高码4K_MP4#单段_高码4K_M3U8#单段_低清_FLV#单段_低清_MP4#单段_低清_M3U8";
        settings.setValue("app/preferQualitys",preferQualitysSetting);
    }


    connect(browseWidget,SIGNAL(play(QString)),this,SLOT(addMvToPlaylist(QString)));

    connect(playerWidget,SIGNAL(hideToFullScreen(bool)),this,SLOT(getIntofullScreenMode(bool)));
    connect(playerWidget,SIGNAL(getIntoWideModel(bool)),this,SLOT(getIntoWideModel(bool)));

    connect(magnetWidget,SIGNAL(addDownloadTask(QString)),this,SLOT(addDownloadTask(QString)));
    connect(recommendWidget,SIGNAL(addDownloadTaskSignal(QString)),this,SLOT(addDownloadTask(QString)));

    connect(getNavgationBar(),SIGNAL(indexChanged(int)),this,SLOT(firstLoadList(int)));

    hideMouseTimer = new QTimer;
    connect(hideMouseTimer,SIGNAL(timeout()),this,SLOT(hideMouse()));
    hideMouseTimer->start(500);

    loadDownloadSettings();
    //setAlignment(LeftTop);
}

void CenterWindow::addMvToPlaylist(QString tvUrl)
{
    myTipWin.show();
    myTipWin.setBusyIndicatorShow(true);
    myTipWin.setText("请求数据中");
    QNetworkAccessManager *manager= new QNetworkAccessManager;
    QNetworkReply *reply = manager->get(QNetworkRequest(QUrl(tvUrl)));
    QEventLoop loop;
    QTimer::singleShot(10000,&loop,SLOT(quit()));
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
    QByteArray data = reply->readAll();

    QStringList tvNoHrefList;
    //find tvName(title) and QStringList serials//来源名称及剧集列表
    QRegExp rx_title("<span class=\"title\">.*</span>");
    rx_title.setMinimal(true);
    int pos = 0;
    QString title;
    while ((pos = rx_title.indexIn(data, pos)) != -1) {
        title=rx_title.cap(0);
        pos += rx_title.matchedLength();
    }
    QString tvName = title.replace(QRegExp("<span class=\"title\">|</span>"),"");

    if(data.contains("好看的综艺"))//综艺大分类
    {
        //取出地址中的影视ID,http://www.yunfan.com/show/va/6287162973540335925.html
        QString id = tvUrl.split("/").last().replace(".html","");
        QRegExp rx_vlist("\"tv_ico\".*</div>");
        QRegExp rx_vlist_one("source=\'.*\'");
        QStringList vlist;
        rx_vlist.setMinimal(true);
        rx_vlist_one.setMinimal(true);
        pos = 0;
        QString vlistStr;
        while ((pos = rx_vlist.indexIn(data, pos)) != -1) {
            vlistStr=rx_vlist.cap(0);
            pos += rx_vlist.matchedLength();
        }
        pos=0;
        while ((pos = rx_vlist_one.indexIn(vlistStr, pos)) != -1) {
            QString str=rx_vlist_one.cap(0);
            vlist.append(str.replace(QRegExp("source=\'|\'"),""));
            pos += rx_vlist_one.matchedLength();
        }

        foreach (QString v, vlist) {
            QString tvNoHrefListStr;//tvNoHrefList<<tvNoHrefListStr,tvNoHrefListStr:1#href
            QString jsonUrl = QString("http://www.yunfan.com/show/vapage.php?id=%1&site=%2").arg(id).arg(v);
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
            }
            tvNoHrefList<<tvNoHrefListStr;

        }

        if(vlist.count()<1)
        {
//            QLabel *lb= new QLabel("无法找到可播放的源，可能该视频收费!\n你可以试着搜索一下。");
//            mainLayout->addWidget(lb,0,0,2,4);
            myTipWin.setText("无播放源");
        }
    }else if(data.contains("tv_ico"))//电视剧或动漫
    {
        //以view-source:http://www.yunfan.com/show/tv/6026173063460419400.html为例，先取出所有<div class="tv_ico">与</div>间的source=xxx内容
        QRegExp rx_vlist("\"tv_ico\".*</div>");
        QRegExp rx_vlist_one("source=\'.*\'");
        QStringList vlist;
        rx_vlist.setMinimal(true);
        rx_vlist_one.setMinimal(true);
        pos = 0;
        QString vlistStr;
        while ((pos = rx_vlist.indexIn(data, pos)) != -1) {
            vlistStr=rx_vlist.cap(0);
            pos += rx_vlist.matchedLength();
        }

        pos=0;
        while ((pos = rx_vlist_one.indexIn(vlistStr, pos)) != -1) {
            QString str=rx_vlist_one.cap(0);
            vlist.append(str.replace(QRegExp("source=\'|\'"),""));
            pos += rx_vlist_one.matchedLength();
        }

        foreach (QString v, vlist) {
            QString tvNoHrefListStr;//
            QRegExp rx_serial(QString("source_%1.*source=\'%2\'.*</div>").arg(v).arg(v));
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
            tvNoHrefList<<tvNoHrefListStr;
        }
        if(vlist.count()<1)
        {
//            QLabel *lb= new QLabel("无法找到可播放的源，可能该视频收费!\n你可以试着搜索一下。");
//            mainLayout->addWidget(lb,0,0,2,4);
            myTipWin.setBusyIndicatorShow(false);
            myTipWin.setText("无播放源");
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
//            QLabel *lb= new QLabel("无法找到可播放的源，可能该视频收费!\n你可以试着搜索一下。");
//            mainLayout->addWidget(lb,0,0,2,4);
            myTipWin.setBusyIndicatorShow(false);
            myTipWin.setText("无播放源");
        }else
        {
            foreach (QString a, lylist) {
                QString href;//视频网站播放地址
                QString source;//来源视频网站名称
                QRegExp rx_temp("href=\'.*\'");
                rx_temp.setMinimal(true);
                pos = 0;
                while ((pos = rx_temp.indexIn(a, pos)) != -1) {
                    href=rx_temp.cap(0);
                    href.remove(QRegExp("href=\'|\'"));
                    pos += rx_temp.matchedLength();
                }
                QRegExp rx_temp2("source=\'.*\'");
                rx_temp2.setMinimal(true);
                pos = 0;
                while ((pos = rx_temp2.indexIn(a, pos)) != -1) {
                    source=rx_temp2.cap(0);
                    source.remove(QRegExp("source=\'|\'"));
                    pos += rx_temp2.matchedLength();
                }
                if(href=="")
                {
                    continue;
                }
                tvNoHrefList<<"1#"+href.split("#").value(0,"");
            }
        }


    }

    QSettings settings("MvGather", "xusongjie");
    QString preferSource = settings.value("app/preferSource", "letv#youku#tudou#imgotv#fun#funsion#hunantv#qq#qiyi#sohu#pptv#pps#1905").toString();
    bool select = false;
    foreach (QString var, preferSource.split("#")) {
        foreach (QString listStr, tvNoHrefList) {
            if(listStr.contains(var))
            {
                select = true;
                QString source = var;
                //tvId:该视频唯一编号,tvName:视频中文名.tvno_hrefs:集数与相应地址...historyNo:上次观看到的集数,tvUrl:source url;source:视频来源标识
                qint64 tvId = QDateTime::currentMSecsSinceEpoch();
                QSqlQuery query_insert;
                query_insert.exec(QString("INSERT INTO playlistTB VALUES ('%1','%2','%3','%4','','%5','%6')").arg(tvId).arg(tvName).arg(listStr).arg("1").arg(tvUrl).arg(source));
                setIndex0();
                playerWidget->play(QString::number(tvId),"1");
                break;
            }
        }
        if(select)
        {
            break;
        }
    }
    myTipWin.timeToHide(1);

}

void CenterWindow::getIntofullScreenMode(bool b)
{
    emit getIntofullScreenSignal(b);
    getNavgationBar()->setVisible(b);
    repaint();
}

void CenterWindow::getIntoWideModel(bool b)
{
    emit getIntoWideSignal(b);
    getNavgationBar()->setVisible(b);
    repaint();
}

void CenterWindow::switchToPlayerAndPlay(QString fileSavePath)
{
    setIndex0();
    playerWidget->playerWin->player->stop();
    playerWidget->playerWin->player->play(fileSavePath);
    playerWidget->playerWin->state = 1;
}

void CenterWindow::checkVersion()
{
    QNetworkAccessManager *manager = new QNetworkAccessManager;
    QNetworkReply *reply = manager->get(QNetworkRequest(QUrl("http://mvgather.com/mvgather/checkVersion.php")));
    QEventLoop loop;
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
    QByteArray data = reply->readAll();
    if(data == "")
    {
        //        QMessageBox msgBox;
        //        msgBox.setText("检查更新失败，请检查网络状况，或到软件主页下载。");
        //        msgBox.exec();
        myTipWin.show();
        myTipWin.setText("检查失败。");
        myTipWin.timeToHide(3);

    }else
    {
        QString v = QString(data.split('=').value(1,""));
        QString vLatest = v;
        QString vThis = version;
        vLatest.replace(".","");
        vThis.replace(".","");
        if(vLatest>vThis)
        {
            //有新版本
            QMessageBox::StandardButton rb  = QMessageBox::question(this, tr("询问"),
                                                                    tr("发现新版本.\n"
                                                                       "您想要下载更新吗？"),
                                                                    QMessageBox::Cancel | QMessageBox::Ok);
            if(rb == QMessageBox::Ok)
            {

                QNetworkReply *reply1 = manager->get(QNetworkRequest(QUrl("http://mvgather.com/mvgather/downloadAddress.php")));
                QEventLoop loop1;
                QObject::connect(reply1, SIGNAL(finished()), &loop1, SLOT(quit()));
                loop1.exec();
                QByteArray data = reply1->readAll();
                if(! QDesktopServices::openUrl(QUrl(QString(data), QUrl::TolerantMode)))
                {
                    //                    QMessageBox msgBox;
                    //                    msgBox.setText("调用浏览器失败，下载地址已复制到剪贴板！");
                    //                    msgBox.exec();
                    myTipWin.show();
                    myTipWin.setText("调用浏览器失败.");
                    myTipWin.timeToHide(3);
                    QClipboard *board = QApplication::clipboard();
                    board->setText(QString(data));
                }

            }
        }else
        {
            //            QMessageBox msgBox;
            //            msgBox.setText(QString("当前已是最新版本：%1").arg(v));
            //            msgBox.exec();
            myTipWin.show();
            myTipWin.setText(QString("当前已是最新版本"));
            myTipWin.timeToHide(3);
        }

    }
    QSettings settings("MvGather", "xusongjie");
    qint64 nowTime = QDateTime::currentMSecsSinceEpoch();
    settings.setValue("app/checkTime", nowTime);
}

void CenterWindow::timeToCheckV()
{
    QSettings settings("MvGather", "xusongjie");
    qint64 LastCheckTime=settings.value("app/checkTime", 0).toLongLong();
    qint64 nowTime = QDateTime::currentMSecsSinceEpoch();
    if(nowTime > (LastCheckTime +7*86400000))
    {
        QTimer::singleShot(10000,this,SLOT(checkVersion()));
    }
}

void CenterWindow::play(QString fileSavePath)
{
    switchToPlayerAndPlay(fileSavePath);
}

void CenterWindow::firstLoadList(int i)
{
    if(browseWidget->isFirstLoad && i==1)
    {
        QTimer::singleShot(600,browseWidget,SLOT(updateShowWidget()));
        //browseWidget->updateShowWidget();
        browseWidget->isFirstLoad = false;
    }else if(recommendWidget->isFirstLoad && i==2)
    {
        //recommendWidget->loadData();
        QTimer::singleShot(600,recommendWidget,SLOT(loadData()));
        recommendWidget->isFirstLoad = false;
    }else if(magnetWidget->isFirstLoad && i==3)
    {
        QTimer::singleShot(600,magnetWidget,SLOT(getMp4baMagnetList()));
        magnetWidget->isFirstLoad = false;
        //magnetWidget->getMp4baMagnetList();
    }
}

void CenterWindow::addDownloadTask(QString url)
{
    QStringList urlList = url.split("#");
    foreach (QString urlForDownload, urlList) {
        QDir dir;
        QDir dir2(dir.homePath()+"/视频");
        QDir dir3(dir.homePath()+"/Videos");
        QString downloadPath;
        if(dir2.exists())
        {
            downloadPath = dir.homePath()+"/视频/MvGather/download";
        }else if(dir3.exists())
        {
            downloadPath = dir.homePath()+"/Videos/MvGather/download";
        }else
        {
            downloadPath = dir.homePath()+"/MvGather/download";
        }
        QString fileSavePath;
        if(urlForDownload.contains("baidupcs.com"))
        {
            QNetworkAccessManager *manager = new QNetworkAccessManager;
            QEventLoop loop;
            QNetworkReply *reply = manager->head(QNetworkRequest(url));
            connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
            loop.exec();
            QVariant var = reply->rawHeader("Content-Disposition");
            QString filename;
            QRegExp rx("filename=\".*\"");
            rx.setMinimal(true);
            //            foreach (QByteArray a, reply->rawHeaderList()) {
            //                QByteArray b =  reply->rawHeader(a);
            //                qDebug()<<a<<":"<<b;
            //            }
            int pos = rx.indexIn(var.toString());
            if(pos>-1)
            {
                filename = rx.cap(0).remove(QRegExp("filename=\"|\""));
            }else
            {
                filename = QDateTime::currentDateTime().toString("yyyy-MM-dd hh-mm-ss");
            }
            fileSavePath=QString("%0/%1").arg(downloadPath).arg(filename);
        }else
        {
            QString fileName = urlForDownload.split("=").last().split("/").last();
            fileSavePath=QString("%0/%1").arg(downloadPath).arg(fileName);
        }

        qint64 taskId = QDateTime::currentMSecsSinceEpoch();
        downLoadWidget = new DownloadWidget(this,url,fileSavePath,taskId,0);
        downLoadWidget->setObjectName("downloadWidget");
        //downLoadWidget->setStyleSheet("background:transparent;");
        connect(downLoadWidget,SIGNAL(play(QString)),SLOT(play(QString)));
        downLoadWidget->on_startBtn_clicked();
        downloadManageScrollAreaWidgetMainLayout->addWidget(downLoadWidget);
        downloadManageScrollAreaWidget->resize(810,150*downloadManageScrollAreaWidgetMainLayout->count());
        //taskId:创建下载任务的id;url任务原地址;fileSavePath:文件保存目录,percent完成的百分比
        QSqlQuery query_insert;
        query_insert.exec(QString("INSERT INTO dtaskTB VALUES('%0','%1','%2','%3')").arg(taskId).arg(url).arg(fileSavePath).arg(0));

    }
}

void CenterWindow::loadDownloadSettings()
{
    QSqlQuery query_select("select * from dtaskTB");
    query_select.exec();
    while(query_select.next())
    {
        qint64 taskId =query_select.value("taskId").toDouble();//
        QString url =query_select.value("url").toString();
        QString fileSavePath =query_select.value("fileSavePath").toString();
        int percent =query_select.value("percent").toInt();
        downLoadWidget = new DownloadWidget(this,url,fileSavePath,taskId,percent);
        downLoadWidget->setObjectName("downloadWidget");
        //downLoadWidget->setStyleSheet("background:transparent;");
        connect(downLoadWidget,SIGNAL(play(QString)),SLOT(play(QString)));
        downloadManageScrollAreaWidgetMainLayout->addWidget(downLoadWidget);
        downloadManageScrollAreaWidget->resize(810,150*downloadManageScrollAreaWidgetMainLayout->count());

    }
}

void CenterWindow::hideMouse()
{
    QPoint pos = QCursor::pos();
    QDesktopWidget* desktopWidget = QApplication::desktop();
    QRect deskRect = desktopWidget->availableGeometry();
    int screenW = deskRect.width();
    if(playerWidget->playerWin->isFullScreen && pos.x()>screenW*0.95)
    {
        playerWidget->playerWin->renderer->setCursor(Qt::BlankCursor);
    }else
    {
        playerWidget->playerWin->renderer->setCursor(Qt::ArrowCursor);
    }
}

void CenterWindow::setIndex0()
{
    playerWidget->updatePlayList();
    getNavgationBar()->setCurrentIndex(0);
}


