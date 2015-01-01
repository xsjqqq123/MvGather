#include "playerlistarea.h"
#include <QSqlQuery>
#include <QVariant>
#include <QDebug>
#include <QTimer>
#include <QSizePolicy>
#include <QScrollBar>
#include <QMouseEvent>
#include <QMenu>
#include <QDateTime>
#include <QClipboard>
#include <QApplication>
#include <QMessageBox>
#include <QCursor>
#include <QInputDialog>
PlayerListArea::PlayerListArea(QWidget *parent) :
    QScrollArea(parent)
{
    setContextMenuPolicy(Qt::DefaultContextMenu);
//    setMaximumWidth(260);
//    setMinimumWidth(220);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setAttribute(Qt::WA_TranslucentBackground, true);
    mainWidget = new QWidget;
//    QSizePolicy policy;
//    policy.setVerticalPolicy(QSizePolicy::Fixed);
//    mainWidget->setSizePolicy(policy);
    //mainWidget->setMaximumWidth(this->viewport()->width());
    setWidget(mainWidget);
    mainWidgetGLayout = new QGridLayout;
    mainWidget->setLayout(mainWidgetGLayout);
    mainWidgetGLayout->setAlignment(Qt::AlignLeft|Qt::AlignTop);
    //    mainWidgetGLayout->setMargin(0);
    //    mainWidgetGLayout->setSpacing(0);
    //QTimer::singleShot(1000, this, SLOT(updateList()));
    this->setWidgetResizable(true);
    updateList();
    mainWidget->setObjectName("playerListAreaMainWidget");
    //mainWidget->setStyleSheet("#playerListAreaMainWidget{background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 rgba(0, 82, 112, 255), stop:0.5 rgba(0, 211, 197, 255), stop:1 rgba(0, 82, 112, 255));}");
}

void PlayerListArea::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu *menu= new QMenu;
    QAction *openUrlAct = new QAction(tr("添加网页视频播放地址"), this);
    openUrlAct->setToolTip("从剪贴板中读取视频所在页url，进行播放，注意以http://开头！");
    QAction *syncAct = new QAction(tr("同步视频列表"), this);
    connect(openUrlAct, SIGNAL(triggered()), this, SLOT(openUrl()));
    connect(syncAct, SIGNAL(triggered()), this, SLOT(sendSyncMv()));
    menu->addAction(openUrlAct);
    menu->addAction(syncAct);
    menu->exec(QCursor::pos());

}
void PlayerListArea::updateList()
{
    while(mainWidgetGLayout->count() > 0)
    {
        QWidget* widget = mainWidgetGLayout->itemAt(0)->widget();
        mainWidgetGLayout->removeWidget(widget);
        delete widget;
    }
    QSqlQuery query_select("select * from playlistTB ORDER BY tvId DESC");
    query_select.exec();
    while(query_select.next())
    {

        //tvId VARCHAR( 30 ) NOT NULL,tvName VARCHAR( 30 ),tvno_hrefs VARCHAR( 30 ),historyNo VARCHAR( 30 )
        QString tvId =query_select.value("tvId").toString();//
        QString tvName =query_select.value("tvName").toString();
        QString tvno_hrefs =query_select.value("tvno_hrefs").toString();
        QString historyNo =query_select.value("historyNo").toString();
        QString source =query_select.value("source").toString();
        //letv#youku#tudou#imgotv#fun#funsion#hunantv#qq#qiyi#sohu#pptv#pps#1905
        if(source.contains("qiyi"))
        {
            source = "爱奇艺";
        }else if(source.contains("letv"))
        {
            source = "乐视";
        }else if(source.contains("tudou"))//
        {
            source = "土豆";
        }else if(source.contains("youku"))
        {
            source = "优酷";
        }else if(source.contains("imgotv"))
        {
            source = "芒果";
        }else if(source.contains("fun"))
        {
            source = "风行";
        }else if(source.contains("qq"))
        {
            source = "qq影视";
        }else if(source.contains("huashu"))
        {
            source = "华数TV";
        }

        listWidgetItem = new PlayerListWidgetItem(0,tvId,tvName+"("+source+")");
        connect(listWidgetItem,SIGNAL(sendPlay(QString,QString)),this,SLOT(sendPlay(QString,QString)));
        connect(listWidgetItem,SIGNAL(updatePlayList()),this,SLOT(updateList()));
        mainWidgetGLayout->addWidget(listWidgetItem);
        QStringList tvno_hrefList =  tvno_hrefs.split("$");
        tvno_hrefList.removeAll("");
        foreach (QString tvno_href , tvno_hrefList) {
            QStringList tempList = tvno_href.split("#");
            listWidgetItem->addSerial(tempList.first(),historyNo);
        }
        listWidgetItem->setMaximumHeight((listWidgetItem->getRow()+1)*50);

    }
    this->verticalScrollBar()->setSliderPosition(0);
}

void PlayerListArea::sendPlay(QString tvId, QString tvNo)
{
    emit play(tvId,tvNo);

}

void PlayerListArea::openUrl()
{
    bool ok;
    QClipboard *clipboard = QApplication::clipboard();
    QString originalText = clipboard->text();
    QString str = QInputDialog::getText(this, tr("输入需解析播放的url地址"),
                                         tr("Url:"), QLineEdit::Normal,
                                         originalText, &ok);
    if (ok && !str.isEmpty())
    {
        if(!str.startsWith("http://"))
        {
            str.prepend("http://");
        }
        str = "1#"+str;
        //tvId:该视频唯一编号,tvName:视频中文名.tvno_hrefs:集数与相应地址...historyNo:上次观看到的集数
        qint64 tvId = QDateTime::currentMSecsSinceEpoch();
        QSqlQuery query_insert;
        query_insert.exec(QString("INSERT INTO playlistTB VALUES ('%1','%2','%3','%4','','','')").arg(tvId).arg(QDateTime::currentDateTime().toString("yyyy.M.d  h:m:s")).arg(str).arg("0"));

        updateList();
    }

}

void PlayerListArea::sendSyncMv()
{
    emit syncMv();
}
