#include "tvlistwidget.h"
#include <QFile>
#include <QList>
#include <QDir>
TvListWidget::TvListWidget(QWidget *parent) :
    QWidget(parent)
{
    mainLayout = new QGridLayout(this);
    mainLayout->setMargin(0);

    treeWidget = new QTreeWidget(this);
    actGoup = new QActionGroup(0);
    connect(actGoup,SIGNAL(triggered(QAction*)),this,SLOT(play(QAction*)));

    treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);//
    treeWidget->setColumnCount(1);
    treeWidget->setHeaderHidden(true);
    treeWidget->setAttribute(Qt::WA_TranslucentBackground, true);

    connect(treeWidget,SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),this,SLOT(itemDoubleClicked(QTreeWidgetItem*,int)));
    connect(treeWidget,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(customContextMenuRequested(QPoint)));

    mainLayout->addWidget(treeWidget,0,0,1,1);

    QTimer::singleShot(500,this,SLOT(loadTvSources()));



}

void TvListWidget::loadTvSources()
{
//    QFile file("tvSourceJson.php");
//    file.open(QFile::ReadOnly);
//    QByteArray source = file.readAll();
    QByteArray source;

    QDir dir;
    QDir dir2(dir.homePath()+"/视频");
    QDir dir3(dir.homePath()+"/Videos");
    QString sourcePath;
    if(dir2.exists())
    {
        sourcePath = dir.homePath()+"/视频/MvGather/tvSources.json";
    }else if(dir3.exists())
    {
        sourcePath = dir.homePath()+"/Videos/MvGather/tvSources.json";
    }else
    {
        sourcePath = dir.homePath()+"/MvGather/tvSources.json";
    }
    QFile file(sourcePath);
    if(file.exists())
    {
        file.open(QFile::ReadOnly);
        source = file.readAll();
    }else
    {
        QString url("http://www.mvgather.com/mvgather/api/tvSourceJson.php");

        for(int i=0;i<2;i++)
        {
            if(source != "")
            {
                break;
            }
            QNetworkAccessManager *manager = new QNetworkAccessManager(this);
            QNetworkReply *reply = manager->get(QNetworkRequest(QUrl(url)));
            QEventLoop loop;
            QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
            loop.exec();
            source = reply->readAll();
        }

    }


    QJsonParseError *error = new QJsonParseError;
    QJsonDocument doc = QJsonDocument::fromJson(source,error);
    //qDebug()<<error->errorString();
    if(error->error != QJsonParseError::NoError)
    {
        return;
    }
    if(doc.isObject())
    {
        QJsonObject obj_root = doc.object();
        foreach (QString key, obj_root.keys()) {
            QTreeWidgetItem *root =  new QTreeWidgetItem(QStringList()<<key);
            //qDebug()<<key;
            QJsonValue v = obj_root[key];
            QJsonArray array =  v.toArray();
            foreach (QJsonValue tv, array) {
                QJsonObject tv_obj = tv.toObject();
                QString tvName = tv_obj["tvName"].toString();
                QString tvUrl = tv_obj["tvUrl"].toString();
                QTreeWidgetItem *leaf = new QTreeWidgetItem(QStringList()<<tvName);

                leaf->setData(0,Qt::AccessibleDescriptionRole,QVariant(tvUrl));
                root->addChild(leaf);
            }
            treeWidget->addTopLevelItem(root);
        }
    }


    treeWidget->expand(treeWidget->model()->index(treeWidget->indexOfTopLevelItem(treeWidget->findItems("Local TV",Qt::MatchExactly).first()),0));
}

void TvListWidget::itemDoubleClicked(QTreeWidgetItem *item, int i)
{
    QString urls = item->data(0,Qt::AccessibleDescriptionRole).toString();
    QStringList urlList = urls.split("#");
    QString url = urlList.value(0,"");
    emit play(url,"高清");

}

void TvListWidget::customContextMenuRequested(QPoint p)
{
    QTreeWidgetItem *item = treeWidget->itemAt(p);
    if(!item || item->childCount()>0)
    {
        return;
    }
    QString urls = item->data(0,Qt::AccessibleDescriptionRole).toString();
    QStringList urlList = urls.split("#");
    QMenu *menu = new QMenu;
    for(int i=0; i<urlList.count();i++)
    {
        QAction *act = menu->addAction(QString("源： %1").arg(i+1));
        act->setToolTip(urlList.value(i,""));
        act->setData(QVariant(urlList.value(i,"")));
        menu->addAction(act);
        actGoup->addAction(act);

    }
    menu->exec(treeWidget->mapToGlobal(p));


}

void TvListWidget::play(QAction *act)
{
    QString url = act->data().toString();
    emit play(url,"高清");
}
