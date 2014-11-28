#include "selectsourcewidget.h"
#include <QEventLoop>
#include <QRegExp>
#include <QDesktopWidget>
#include <QApplication>
#include <QLabel>
#include <QSqlQuery>
#include <QDateTime>
#include <QDebug>
#include <QTimer>
#include <QSettings>
SelectSourceWidget::SelectSourceWidget(QString tvUrl)
{
    setWindowFlags(Qt::WindowStaysOnTopHint|Qt::FramelessWindowHint);
    //setWindowFlags(Qt::Tool | Qt::WindowStaysOnTopHint);
    getTitleBar()->getTitleLabel()->setText("选择观看来源");
    this->tvUrl=tvUrl;
    btnGroup = new QButtonGroup;
    connect(btnGroup,SIGNAL(buttonClicked(int)),this,SLOT(addTvToPlaylistTB(int)));

    myTipWin.show();
    myTipWin.setBusyIndicatorShow(true);
    myTipWin.setText("正在请求数据");

    manager = new QNetworkAccessManager;
    QNetworkReply *reply = manager->get(QNetworkRequest(QUrl(tvUrl)));
    QEventLoop loop;
    QTimer::singleShot(10000,&loop,SLOT(quit()));
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
    QByteArray data = reply->readAll();

    mainLayout = new QGridLayout;
    mainLayout->setMargin(5);
    mainLayout->setSpacing(5);
    //setLayout(mainLayout);
    this->fMainLayout->addLayout(mainLayout);

    processData(data);
}

void SelectSourceWidget::processData(QByteArray data)
{
    myTipWin.setBusyIndicatorShow(false);
    tvNoHrefList = QStringList();
    //find tvName(title) and QStringList serials//来源名称及剧集列表
    QRegExp rx_title("<span class=\"title\">.*</span>");
    rx_title.setMinimal(true);
    int pos = 0;
    QString title;
    while ((pos = rx_title.indexIn(data, pos)) != -1) {
        title=rx_title.cap(0);
        pos += rx_title.matchedLength();
    }
    this->tvName = title.replace(QRegExp("<span class=\"title\">|</span>"),"");

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
            QTimer::singleShot(1000,this,SLOT(close()));
            myTipWin.setBusyIndicatorShow(false);
            myTipWin.setText("无可播放的源");

        }else
        {
            QLabel *lb= new QLabel("选择一个播放源:");
            mainLayout->addWidget(lb,0,0,2,4);
            int column=0;
            int row = 2;
            int id = 0;
            foreach (QString txt, vlist) {
                btn = new QToolButton;
                btn->setStyleSheet("QToolButton{background-color: rgba(0, 82, 112, 100);color: white;border: 1px;}QToolButton:hover{background-color: rgba(0, 82, 112, 200);color: white;border: 1px;}QToolButton:disabled{background-color: transparent;color: gray;}");
                btnGroup->addButton(btn,id);
                btn->setText(txt);
                btn->setMinimumSize(100,40);
                mainLayout->addWidget(btn,row,column,1,1);
                if(column >1)
                {
                    column=0;
                    row++;
                }else
                {
                    column++;
                }
                id++;
            }
            this->normalSize.setWidth(vlist.count()<=3?vlist.count()*100:300);
            this->normalSize.setHeight(vlist.count()*40/3);
            this->show();
            myTipWin.setText("选择一个播放源");
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
            myTipWin.setText("无可播放的源");
            QTimer::singleShot(1000,this,SLOT(close()));
        }else
        {
            QLabel *lb= new QLabel("选择一个播放源:");
            mainLayout->addWidget(lb,0,0,2,4);
            int column=0;
            int row = 2;
            int id = 0;
            foreach (QString txt, vlist) {
                btn = new QToolButton;
                btn->setStyleSheet("QToolButton{background-color: rgba(0, 82, 112, 100);color: white;border: 1px;}QToolButton:hover{background-color: rgba(0, 82, 112, 200);color: white;border: 1px;}QToolButton:disabled{background-color: transparent;color: gray;}");
                btnGroup->addButton(btn,id);
                btn->setText(txt);
                btn->setMinimumSize(100,40);
                mainLayout->addWidget(btn,row,column,1,1);
                if(column >1)
                {
                    column=0;
                    row++;
                }else
                {
                    column++;
                }
                id++;
            }
            this->normalSize.setWidth(vlist.count()<=3?vlist.count()*100:300);
            this->normalSize.setHeight(vlist.count()*40/3);
            this->show();
            myTipWin.setText("选择一个播放源");
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
            myTipWin.setText("无可播放的源");
            QTimer::singleShot(1000,this,SLOT(close()));
        }else
        {
            int column=0;
            int row = 2;
            int id = 0;
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

                QLabel *lb= new QLabel("选择一个播放源:");
                mainLayout->addWidget(lb,0,0,2,4);
                btn = new QToolButton;
                btn->setStyleSheet("QToolButton{background-color: rgba(0, 82, 112, 100);color: white;border: 1px;}QToolButton:hover{background-color: rgba(0, 82, 112, 200);color: white;border: 1px;}QToolButton:disabled{background-color: transparent;color: gray;}");
                btnGroup->addButton(btn,id);
                btn->setText(source);
                btn->setMinimumSize(100,40);
                mainLayout->addWidget(btn,row,column,1,1);
                if(column >1)
                {
                    column=0;
                    row++;
                }else
                {
                    column++;
                }
                id++;

                this->normalSize.setWidth(lylist.count()<=3?lylist.count()*100:300);
                this->normalSize.setHeight(lylist.count()*40/3);
                this->show();
                myTipWin.setText("选择一个播放源");
            }
        }


    }

    QSettings settings("MvGather", "xusongjie");
    QString preferSource = settings.value("app/preferSource", "letv#qiyi#sohu").toString();
    bool select = false;
    foreach (QString var, preferSource.split("#")) {
        foreach (QAbstractButton *btn, btnGroup->buttons()) {
            if(btn->text().contains(var))
            {
                select = true;
                addTvToPlaylistTB(btnGroup->id(btn));
                break;
            }
        }
        if(select)
        {
            break;
        }
    }

    myTipWin.timeToHide(3);

}

void SelectSourceWidget::addTvToPlaylistTB(int i)
{
    QString source = btnGroup->button(i)->text();
    //tvId:该视频唯一编号,tvName:视频中文名.tvno_hrefs:集数与相应地址...historyNo:上次观看到的集数,tvUrl:source url;source:视频来源标识
    qint64 tvId = QDateTime::currentMSecsSinceEpoch();
    QSqlQuery query_insert;
    query_insert.exec(QString("INSERT INTO playlistTB VALUES ('%1','%2','%3','%4','','%5','%6')").arg(tvId).arg(tvName).arg(this->tvNoHrefList.value(i,"")).arg("0").arg(tvUrl).arg(source));
    emit updatePlayList();
    this->close();
}

