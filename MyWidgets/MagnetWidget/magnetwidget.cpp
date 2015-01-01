#include "magnetwidget.h"
#include "ui_magnetwidget.h"
#include <QDesktopWidget>
#include <QInputDialog>
#include <QMessageBox>
MagnetWidget::MagnetWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MagnetWidget)
{
    ui->setupUi(this);
    ui->magnetSearchTbWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->magnetSearchTbWidget->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    ui->magnetSearchTbWidget->horizontalHeader()->setStretchLastSection(true);
    ui->magnetSearchTbWidget->horizontalHeader()->resizeSection(0,200);
    ui->magnetSearchTbWidget->horizontalHeader()->resizeSection(1,200);

    ui->panMovieListWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->downloadMagnetlistWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->magnetSearchTbWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    ui->frame_4->setMaximumWidth(400);
    ui->frame->setMaximumWidth(400);
    ui->frame_2->setMaximumWidth(400);
    ui->frame->setEnabled(false);
    ui->frame_2->setEnabled(false);
    searchManager = new QNetworkAccessManager(this);
    panManager = new QNetworkAccessManager(this);

    isFirstLoad = true;
}

MagnetWidget::~MagnetWidget()
{
    delete ui;
}

void MagnetWidget::getMp4baMagnetList()
{
    //http://www.mp4ba.com/index.php?page=1
    myTipWin.show();
    myTipWin.setBusyIndicatorShow(true);
    myTipWin.setText("正在加载");
    QEventLoop loop;
    int row = 0;
    for(int i = 1;i<2;i++)
    {
        QNetworkReply *reply = searchManager->get(QNetworkRequest(QUrl(QString("http://www.mp4ba.com/index.php?page=%0").arg(i))));
        QTimer::singleShot(5000,&loop,SLOT(quit()));
        QObject::connect(reply,SIGNAL(finished()), &loop, SLOT(quit()), Qt::DirectConnection);
        loop.exec();
        QString result = reply->readAll();

        QRegExp rx("<tr class.*</tr>");
        QRegExp rx_hash("hash=.*\"");
        QRegExp rx_name("target=\"_blank\">.*<");
        //QRegExp rx_size("<td>\\d{1,3}.*</td>");
        rx.setMinimal(true);
        rx_hash.setMinimal(true);
        rx_name.setMinimal(true);
        //rx_size.setMinimal(true);
        int pos = 0;
        while ((pos = rx.indexIn(result, pos)) != -1) {

            pos+=rx.matchedLength();
            QString file=  rx.cap(0);
            //qDebug()<<file;
            rx_hash.indexIn(file);
            QString hash = rx_hash.cap(0).replace(QRegExp("hash=|\""),"");
            if(hash.isEmpty())
            {
                continue;
            }
            rx_name.indexIn(file);
            QString name = rx_name.cap(0).replace(QRegExp("target=\"_blank\">|<"),"").trimmed();
            rx_name.indexIn(file);
//            QString size = rx_size.cap(0).replace(QRegExp("<td>|</td>"),"");
//            pos += rx.matchedLength();
            QTableWidgetItem * item0 = new QTableWidgetItem(name);
            QTableWidgetItem * item1 = new QTableWidgetItem("来自高清MP4吧");
            QTableWidgetItem * item2 = new QTableWidgetItem(QString("magnet:?xt=urn:btih:%0&tr=http://bt.mp4ba.com:2710/announce").arg(hash));
            row++;
            ui->magnetSearchTbWidget->setRowCount(row);
            ui->magnetSearchTbWidget->setItem(row-1,0,item0);
            ui->magnetSearchTbWidget->setItem(row-1,1,item1);
            ui->magnetSearchTbWidget->setItem(row-1,2,item2);


        }


    }
    myTipWin.setText("加载结束");
    myTipWin.timeToHide(1);
}

void MagnetWidget::on_searchLineEdit_returnPressed()
{
    getMagnetList(ui->searchLineEdit->text());
}

void MagnetWidget::getMagnetList(QString keyWord)
{
    //
    myTipWin.show();
    myTipWin.setBusyIndicatorShow(true);
    myTipWin.setText("正在搜索");
    ui->magnetSearchTbWidget->clearContents();
    ui->magnetSearchTbWidget->setRowCount(0);/*
    //bt.shousibaocai.com
    QEventLoop loop;
    int rowCount = 0;
    for(int i = 1;i<4;i++)
    {
        QNetworkReply *reply = searchManager->get(QNetworkRequest(QUrl(QString("http://bt.shousibaocai.com/list/%0/%1").arg(keyWord).arg(i))));
        QObject::connect(reply,SIGNAL(finished()), &loop, SLOT(quit()), Qt::DirectConnection);
        loop.exec();
        QString data = reply->readAll();

        QRegExp rx_mainData("<table.*</table>");
        rx_mainData.setMinimal(true);
        rx_mainData.indexIn(data);
        data = rx_mainData.cap(0);

        QStringList filesList = data.split("</td>");
        filesList.removeLast();

        QRegExp rx_title("<a class=\"title\" href=\"/info.*</a>");
        QRegExp rx_detail("<div class=\"tail.*</span>");
        QRegExp rx_magnet("magnet.*\"");
        QRegExp rx_remove("<.*>");
        rx_title.setMinimal(true);
        rx_detail.setMinimal(true);
        rx_magnet.setMinimal(true);
        rx_remove.setMinimal(true);
        foreach (QString file, filesList) {
            ui->magnetSearchTbWidget->setRowCount(rowCount+1);
            QString title;
            QString detail;
            QString magnet;
            rx_title.indexIn(file);
            title = rx_title.cap(0).remove(rx_remove);
            rx_detail.indexIn(file);
            detail = rx_detail.cap(0).remove(rx_remove);
            rx_magnet.indexIn(file);
            magnet = rx_magnet.cap(0).remove(QRegExp("\""));
            QTableWidgetItem *itemTitle = new QTableWidgetItem(title);
            QTableWidgetItem *itemDetail = new QTableWidgetItem(detail);
            QTableWidgetItem *itemMagnet = new QTableWidgetItem(magnet);
            ui->magnetSearchTbWidget->setItem(rowCount,0,itemTitle);
            ui->magnetSearchTbWidget->setItem(rowCount,1,itemDetail);
            ui->magnetSearchTbWidget->setItem(rowCount,2,itemMagnet);
            rowCount+=1;
        }
    }*/
    //http://www.btbook.net/
    QEventLoop loop;
    int rowCount = 0;
    for(int i = 1;i<4;i++)
    {
        QNetworkReply *reply = searchManager->get(QNetworkRequest(QUrl(QString("http://www.btbook.net/search/%0/%1").arg(keyWord).arg(i))));
        QObject::connect(reply,SIGNAL(finished()), &loop, SLOT(quit()), Qt::DirectConnection);
        loop.exec();
        QString data = reply->readAll();


        QStringList filesList = data.split("search-item");
        if(!filesList.isEmpty())
        {
            filesList.removeFirst();
        }

        QRegExp rx_title("<p>.*<span>");
        QRegExp rx_detail("<span>.*</span>");
        QRegExp rx_magnet("magnet.*\"");
        QRegExp rx_remove("<.*>");
        QRegExp rx_remove2(" ");
        rx_title.setMinimal(true);
        rx_detail.setMinimal(true);
        rx_magnet.setMinimal(true);
        rx_remove.setMinimal(true);
        rx_remove2.setMinimal(true);
        foreach (QString file, filesList) {
            ui->magnetSearchTbWidget->setRowCount(rowCount+1);
            QString title;
            QString detail("文件大小：");
            QString magnet;
            rx_title.indexIn(file);
            title = rx_title.cap(0).remove(rx_remove);
            rx_detail.indexIn(file);
            detail += rx_detail.cap(0).remove(rx_remove);
            rx_magnet.indexIn(file);
            magnet = rx_magnet.cap(0).remove(QRegExp("\""));
            QTableWidgetItem *itemTitle = new QTableWidgetItem(title);
            QTableWidgetItem *itemDetail = new QTableWidgetItem(detail);
            QTableWidgetItem *itemMagnet = new QTableWidgetItem(magnet);
            ui->magnetSearchTbWidget->setItem(rowCount,0,itemTitle);
            ui->magnetSearchTbWidget->setItem(rowCount,1,itemDetail);
            ui->magnetSearchTbWidget->setItem(rowCount,2,itemMagnet);
            rowCount+=1;
        }
    }
    myTipWin.setText("搜索结束");
    myTipWin.timeToHide(2);
}

void MagnetWidget::setPanInfo(QString text)
{
    ui->panSizeLbl->setText(text);
    //qDebug()<<text;
}

void MagnetWidget::getRealUrl(QString path)
{
    myTipWin.show();
    myTipWin.setText("正在建立任务");
    myTipWin.setBusyIndicatorShow(true);
    path = path.toUtf8().toPercentEncoding();
    QString url = QString("http://d.pcs.baidu.com/rest/2.0/pcs/file?method=download&path=%0&app_id=250528").arg(path);

    int tryTimes=5;
    //尝试tryTimes次
    QString realUrl_temp = url;
    while(tryTimes --)
    {
        QEventLoop loop;
        QNetworkReply *reply = panManager->head(QNetworkRequest(realUrl_temp));
        connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
        loop.exec();
        QVariant varLocation = reply->header(QNetworkRequest::LocationHeader);
//        foreach (QByteArray var, reply->rawHeaderList()) {
//            qDebug()<<var<<"+++"<<reply->rawHeader(var);
//        }
        if(!varLocation.isNull())
        {
            realUrl_temp = varLocation.toString();
            continue;
        }
        delete reply;
        break;
    }
    url = realUrl_temp;
    myTipWin.timeToHide(0);
    emit addDownloadTask(url);
}

void MagnetWidget::newMagnetTask(QString url)
{
    myTipWin.show();
    myTipWin.setBusyIndicatorShow(true);
    myTipWin.setText("获取磁链信息");
    //获取磁链的信息.
    QStringList fileToDownMark;
    QEventLoop loop;
    QByteArray postDownloadMagnetInfoData;
    postDownloadMagnetInfoData.append("method=query_magnetinfo&app_id=250528&");
    postDownloadMagnetInfoData.append(QString("source_url=%0&").arg(url));
    postDownloadMagnetInfoData.append("&save_path=/MvGather&");
    postDownloadMagnetInfoData.append("type=4");
    QNetworkRequest request(QUrl(QString("http://pan.baidu.com/rest/2.0/services/cloud_dl?channel=chunlei&clienttype=0&web=1&bdstoken=%0").arg(token)));
    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");
    QNetworkReply *replyDownloadMagnetInfo = panManager->post(request,postDownloadMagnetInfoData);
    QObject::connect(replyDownloadMagnetInfo,SIGNAL(finished()), &loop, SLOT(quit()), Qt::DirectConnection);
    loop.exec();
    QByteArray downloadMagnetInfoResult = replyDownloadMagnetInfo->readAll();
    //qDebug()<<downloadMagnetInfoResult;

    QJsonParseError *error=new QJsonParseError;
    QJsonDocument doc=QJsonDocument::fromJson(downloadMagnetInfoResult,error);
    if(error->error==QJsonParseError::NoError)
    {
        QJsonObject rootObj = doc.object();
        QVariantMap magnet_infoMap = rootObj.toVariantMap();
        int total = magnet_infoMap.value("total").toInt();
        if(total > 0)
        {
            int mark = 1;
            QJsonArray fileListArray = rootObj["magnet_info"].toArray();
            //qDebug()<<magnet_infoMap.value("magnet_info");
            foreach (QJsonValue magnetFileV, fileListArray) {
                QVariantMap fileMap = magnetFileV.toObject().toVariantMap();
                //qDebug()<<fileMap;
                if(fileMap.value("size").toDouble()>(1024.0*1024.0*20.0))// >20 MB
                {
                    fileToDownMark.append(QString::number(mark));
                }
                mark++;
            }
        }
        //qDebug()<<fileToDownMark;
    }

    //下载magent磁链.
    QString vcode;
    QString vcodeImgUrl;
    for(int i = 0;i<3;i++)
    {
        QByteArray postDownloadMagnetData;
        postDownloadMagnetData.append(QString("method=add_task&app_id=250528&file_sha1=&save_path=/MvGather&selected_idx=%0&task_from=1&").arg(fileToDownMark.join(",")));
        postDownloadMagnetData.append(QString("source_url=%0&").arg(url));
        postDownloadMagnetData.append("type=4");
        if(!vcode.isEmpty())
        {

            postDownloadMagnetData.append(QString("&vcode=%0&").arg(vcode));
            //user input vcode
            QString vcode_input("abcd");

            QEventLoop loop;
            QNetworkReply *reply = panManager->get(QNetworkRequest(QUrl(vcodeImgUrl)));
            QObject::connect(reply,SIGNAL(finished()), &loop, SLOT(quit()), Qt::DirectConnection);
            loop.exec();
            QByteArray imgData = reply->readAll();
            QPixmap pix;
            pix.loadFromData(imgData);

            VcodeDialog *vcodeDialog = new VcodeDialog(0);
            vcodeDialog->setWindowTitle("请输入验证码");
            vcodeDialog->setPixmap(pix);
            int i = vcodeDialog->exec();
            if(i == -1)
            {
                break;
            }else
            {
                vcode_input = vcodeDialog->getText();
            }
            postDownloadMagnetData.append(QString("&input=%0").arg(vcode_input));
        }
        QNetworkRequest request(QUrl(QString("http://pan.baidu.com/rest/2.0/services/cloud_dl?channel=chunlei&clienttype=0&web=1&bdstoken=%0").arg(token)));
        request.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");
        QNetworkReply *replyDownloadMagnet = panManager->post(request,postDownloadMagnetData);
        QObject::connect(replyDownloadMagnet,SIGNAL(finished()), &loop, SLOT(quit()), Qt::DirectConnection);
        loop.exec();
        QByteArray downloadMagnetResult = replyDownloadMagnet->readAll();
        //qDebug()<<downloadMagnetResult;
        if(downloadMagnetResult.contains("error_code\":-19"))//需要验证码
        {
            QJsonParseError *error=new QJsonParseError;
            QJsonDocument doc=QJsonDocument::fromJson(downloadMagnetResult,error);
            if(error->error==QJsonParseError::NoError)
            {
                QJsonObject rootObj = doc.object();
                QVariantMap rootMap = rootObj.toVariantMap();
                vcode = rootMap.value("vcode").toString();
                //qDebug()<<rootMap;
                vcodeImgUrl = rootMap.value("img").toString();
            }
        }else if(downloadMagnetResult.contains("36028"))//磁力链无法解析
        {
            myTipWin.setBusyIndicatorShow(false);
            myTipWin.setText("磁力链无法解析");
            break;
        }else if(downloadMagnetResult.contains("36032"))//离线文件因含有违规内容被系统屏蔽无法下载
        {
            myTipWin.setBusyIndicatorShow(false);
            myTipWin.setText("含有违规内容");
            break;
        }else
        {
            QTimer::singleShot(2000,this,SLOT(on_refreshTaskBtn_clicked()));
            QTimer::singleShot(4000,this,SLOT(on_refreshFilesList_clicked()));
            break;
        }
    }
    myTipWin.timeToHide(1);
}

void MagnetWidget::newUrlTask(QString UrlText)
{
    myTipWin.show();
    myTipWin.setBusyIndicatorShow(true);
    myTipWin.setText("获取Url信息");
    QString vcode;
    QString vcodeImgUrl;
    QEventLoop loop;
    for(int i = 0;i<3;i++)
    {
        QByteArray postDownloadUrlData;
        postDownloadUrlData.append(QString("method=add_task&app_id=250528&file_sha1=&save_path=/MvGather&"));
        postDownloadUrlData.append(QString("source_url=%0").arg(UrlText));
        if(!vcode.isEmpty())
        {

            postDownloadUrlData.append(QString("&vcode=%0&").arg(vcode));
            //user input vcode
            QString vcode_input("abcd");

            QEventLoop loop;
            QNetworkReply *reply = panManager->get(QNetworkRequest(QUrl(vcodeImgUrl)));
            QObject::connect(reply,SIGNAL(finished()), &loop, SLOT(quit()), Qt::DirectConnection);
            loop.exec();
            QByteArray imgData = reply->readAll();
            QPixmap pix;
            pix.loadFromData(imgData);

            VcodeDialog *vcodeDialog = new VcodeDialog(0);
            vcodeDialog->setWindowTitle("请输入验证码");
            vcodeDialog->setPixmap(pix);
            int i = vcodeDialog->exec();
            if(i == -1)
            {
                break;
            }else
            {
                vcode_input = vcodeDialog->getText();
            }
            postDownloadUrlData.append(QString("&input=%0").arg(vcode_input));
        }
        QNetworkRequest request(QUrl(QString("http://pan.baidu.com/rest/2.0/services/cloud_dl?channel=chunlei&clienttype=0&web=1&bdstoken=%0").arg(token)));
        request.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");
        QNetworkReply *replyDownloadUrl = panManager->post(request,postDownloadUrlData);
        QObject::connect(replyDownloadUrl,SIGNAL(finished()), &loop, SLOT(quit()), Qt::DirectConnection);
        loop.exec();
        QByteArray downloadUrlResult = replyDownloadUrl->readAll();
        //qDebug()<<downloadMagnetResult;
        if(downloadUrlResult.contains("error_code\":-19"))//需要验证码
        {
            QJsonParseError *error=new QJsonParseError;
            QJsonDocument doc=QJsonDocument::fromJson(downloadUrlResult,error);
            if(error->error==QJsonParseError::NoError)
            {
                QJsonObject rootObj = doc.object();
                QVariantMap rootMap = rootObj.toVariantMap();
                vcode = rootMap.value("vcode").toString();
                //qDebug()<<rootMap;
                vcodeImgUrl = rootMap.value("img").toString();
            }
        }else if(downloadUrlResult.contains("36028"))//磁力链无法解析
        {
            myTipWin.setBusyIndicatorShow(false);
            myTipWin.setText("Url无法解析");
            break;
        }else if(downloadUrlResult.contains("36032"))//离线文件因含有违规内容被系统屏蔽无法下载
        {
            myTipWin.setBusyIndicatorShow(false);
            myTipWin.setText("含有违规内容");
            break;
        }else
        {
            QTimer::singleShot(2000,this,SLOT(on_refreshTaskBtn_clicked()));
            QTimer::singleShot(4000,this,SLOT(on_refreshFilesList_clicked()));
            break;
        }
    }
    myTipWin.timeToHide(1);
}

void MagnetWidget::on_searchBtn_clicked()
{
    getMagnetList(ui->searchLineEdit->text());
}

void MagnetWidget::on_loginBtn_clicked()
{
    if(loginFormWin.loginForm->isVisible())
    {
        return;
    }
    ui->loginBtn->setEnabled(false);

    myTipWin.show();
    myTipWin.setBusyIndicatorShow(true);
    myTipWin.setText("请稍候");
    if(!loginFormWin.loginForm->isInitMark)
    {
        loginFormWin.loginForm->init(panManager);
        //QDesktopWidget* desktop = qApp->desktop();
        //loginForm.move((desktop->width() - loginForm.width())/2, (desktop->height() - loginForm.height())/3);
        connect(loginFormWin.loginForm,SIGNAL(panInfoChanger(QString)),this,SLOT(setPanInfo(QString)));
        connect(loginFormWin.loginForm,SIGNAL(loginSuccess(QString,QString)),this,SLOT(loginSuccess(QString,QString)));
    }
    loginFormWin.show();
    myTipWin.timeToHide(0);
    ui->loginBtn->setEnabled(true);
}

void MagnetWidget::loginSuccess(QString userName, QString token)
{
    this->userName = userName;
    ui->usrNameLbl->setText(userName);
    this->token = token;
    ui->loginBtn->setVisible(false);
    ui->frame->setEnabled(true);
    ui->frame_2->setEnabled(true);

    on_refreshTaskBtn_clicked();
    on_refreshFilesList_clicked();
}

void MagnetWidget::on_panMovieListWidget_doubleClicked(const QModelIndex &index)
{
    QString path = ui->panMovieListWidget->currentItem()->text();
    getRealUrl(path);
}

void MagnetWidget::on_panMovieListWidget_customContextMenuRequested(const QPoint &pos)
{
    QListWidgetItem *item = ui->panMovieListWidget->currentItem();
    if(item == Q_NULLPTR)
    {
        return;
    }
    QString path = item->text();

    QMenu *menu =  new QMenu(0);
    QAction *downloadAct = menu->addAction("下载");
    //QAction *deleteAct = menu->addAction("删除网盘文件");
    QAction *returnAct = menu->exec(mapFromParent(QCursor::pos()));
    if(returnAct == downloadAct)
    {
        getRealUrl(path);
    }/*else if(returnAct == deleteAct)
    {
        QEventLoop loop;
        QByteArray postDeleteTaskData;
        postDeleteTaskData.append(QString("filelist=%0").arg(QString("[\"%0\"]").arg(path)));
        QNetworkReply *replyDeleteTask = panManager->post(QNetworkRequest(QUrl(QString("http://pan.baidu.com/api/filemanager?channel=chunlei&clienttype=0&web=1&opera=delete&async=1&bdstoken=%0&channel=chunlei&clienttype=0&web=1&app_id=250528").arg(token))),postDeleteTaskData);
        QObject::connect(replyDeleteTask,SIGNAL(finished()), &loop, SLOT(quit()), Qt::DirectConnection);
        loop.exec();
        QByteArray Result = replyDeleteTask->readAll();
        on_refreshPanToolBtn_clicked();

    }*/

}

void MagnetWidget::on_clearTaskBtn_clicked()
{
    ui->clearTaskBtn->setEnabled(false);

    QEventLoop loop;
    QNetworkReply *replyClearTasklist = panManager->get(QNetworkRequest(QUrl(QString("http://pan.baidu.com/rest/2.0/services/cloud_dl?bdstoken=%0&method=clear_task&app_id=250528&t=1415008533508&channel=chunlei&clienttype=0&web=1&app_id=250528").arg(token))));
    QTimer::singleShot(10000,&loop, SLOT(quit()));
    QObject::connect(replyClearTasklist,SIGNAL(finished()), &loop, SLOT(quit()), Qt::DirectConnection);
    loop.exec();
    QByteArray result = replyClearTasklist->readAll();

    ui->clearTaskBtn->setEnabled(true);
    on_refreshTaskBtn_clicked();
}

void MagnetWidget::on_downloadMagnetlistWidget_customContextMenuRequested(const QPoint &pos)
{
    QListWidgetItem *item = ui->downloadMagnetlistWidget->currentItem();
    if(item == Q_NULLPTR)
    {
        return;
    }
    if(item->text().contains("未完成"))
    {
        QMenu *menu = new QMenu;
        QAction *cancelTaskAct = menu->addAction("取消当前项下载");
        QAction *returnAct = menu->exec(QCursor::pos());
        if(returnAct == cancelTaskAct)
        {
            QEventLoop loop;
            QNetworkReply *replyCancelTasklist = panManager->get(QNetworkRequest(QUrl(QString("http://pan.baidu.com/rest/2.0/services/cloud_dl?bdstoken=%0&task_id=%1&method=cancel_task&app_id=250528&t=1415010139431&channel=chunlei&clienttype=0&web=1&app_id=250528").arg(token).arg(item->data(Qt::UserRole).toString()))));
            QObject::connect(replyCancelTasklist,SIGNAL(finished()), &loop, SLOT(quit()), Qt::DirectConnection);
            loop.exec();
            QByteArray result = replyCancelTasklist->readAll();
            on_refreshTaskBtn_clicked();
        }

    }
}


void MagnetWidget::on_magnetSearchTbWidget_customContextMenuRequested(const QPoint &pos)
{
    int row = ui->magnetSearchTbWidget->currentRow();
    QTableWidgetItem *item;
    if(row>-1)
    {
         item = ui->magnetSearchTbWidget->item(row,2);
    }else
    {
        return;
    }

    if(item == Q_NULLPTR)
    {
        return;
    }
    QMenu *menu = new QMenu;
    QAction *addTaskAct = menu->addAction("百度云离线下载此链接");
    QAction *returnAct = menu->exec(QCursor::pos());
    if(returnAct == addTaskAct)
    {
        if(this->token.isEmpty())
        {
            myTipWin.show();
            myTipWin.setBusyIndicatorShow(false);
            myTipWin.setText("请先登录百度云");
            myTipWin.timeToHide(2);
            return;
        }
        newMagnetTask(item->text());

    }
}

void MagnetWidget::on_deleteFiles_clicked()
{
    ui->deleteFiles->setEnabled(false);
    if(QMessageBox::Yes == QMessageBox::question(0,"询问", "确定要删除目录内所有文件？"))
    {
        QEventLoop loop;
        QByteArray postDeleteTaskData;
        postDeleteTaskData.append(QString("filelist=%0").arg(QString("[\"/MvGather\"]")));
        QNetworkRequest request(QUrl(QString("http://pan.baidu.com/api/filemanager?channel=chunlei&clienttype=0&web=1&opera=delete&async=1&bdstoken=%0&channel=chunlei&clienttype=0&web=1&app_id=250528").arg(token)));
        request.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");
        QNetworkReply *replyDeleteTask = panManager->post(request,postDeleteTaskData);
        QTimer::singleShot(10000,&loop, SLOT(quit()));
        QObject::connect(replyDeleteTask,SIGNAL(finished()), &loop, SLOT(quit()), Qt::DirectConnection);
        loop.exec();
        QByteArray Result = replyDeleteTask->readAll();
        //qDebug()<<Result;
        ui->deleteFiles->setEnabled(true);
        on_refreshFilesList_clicked();
    }
    ui->deleteFiles->setEnabled(true);
}

void MagnetWidget::on_newMagnet_clicked()
{
    bool ok;
    QString magnetText = QInputDialog::getText(this, tr("请输入磁力链接"),
                                               tr("链接地址:"), QLineEdit::Normal,
                                               "magnet:", &ok);
    if (ok && !magnetText.isEmpty())
    {
        newMagnetTask(magnetText);
    }
}

void MagnetWidget::on_refreshTaskBtn_clicked()
{
    ui->refreshTaskBtn->setEnabled(false);
    QEventLoop loop;
    QNetworkReply *replyDownloadMagnetlistInfo = panManager->get(QNetworkRequest(QUrl(QString("http://pan.baidu.com/rest/2.0/services/cloud_dl?channel=chunlei&clienttype=0&web=1&bdstoken=%0&need_task_info=1&status=255&start=0&limit=50&method=list_task&app_id=250528").arg(token))));
    QTimer::singleShot(10000,&loop, SLOT(quit()));
    QObject::connect(replyDownloadMagnetlistInfo,SIGNAL(finished()), &loop, SLOT(quit()), Qt::DirectConnection);
    loop.exec();
    QByteArray DownloadMagnetlist = replyDownloadMagnetlistInfo->readAll();
    //qDebug()<<DownloadMagnetlist;
    QJsonParseError *error=new QJsonParseError;
    QJsonDocument doc=QJsonDocument::fromJson(DownloadMagnetlist,error);
    if(error->error==QJsonParseError::NoError)
    {
        ui->downloadMagnetlistWidget->clear();
        QJsonObject rootObj= doc.object();
        //QVariantMap rootMap = rootObj.toVariantMap();
        QJsonArray listArray = rootObj["task_info"].toArray();
        foreach (QJsonValue fileV, listArray) {

            QJsonObject fileObj = fileV.toObject();
            QVariantMap fileObjMap = fileObj.toVariantMap();
            QString task_name = fileObjMap.value("task_name").toString();
            QString status = fileObjMap.value("status").toString();
            QString task_id = fileObjMap.value("task_id").toString();
            QListWidgetItem * item = new QListWidgetItem((status=="0"?"(已完成)":"(未完成)")+task_name);
            item->setData(Qt::UserRole,task_id);
            ui->downloadMagnetlistWidget->addItem(item);
        }
    }
    ui->refreshTaskBtn->setEnabled(true);
}

void MagnetWidget::on_refreshFilesList_clicked()
{
    ui->refreshFilesList->setEnabled(false);
    QEventLoop loop;
    //http://pan.baidu.com/api/list?channel=chunlei&clienttype=0&web=1&num=10&page=1&dir=/MvGather&order=time&desc=1&bdstoken=
    QNetworkReply *replyPanFileListInfo = panManager->get(QNetworkRequest(QUrl(QString("http://pan.baidu.com/api/list?channel=chunlei&clienttype=0&web=1&num=50&page=1&dir=/MvGather&order=time&desc=1&bdstoken=%0&_=%1").arg(token).arg(QString::number(QDateTime::currentMSecsSinceEpoch())))));
    QTimer::singleShot(10000,&loop, SLOT(quit()));
    QObject::connect(replyPanFileListInfo,SIGNAL(finished()), &loop, SLOT(quit()), Qt::DirectConnection);
    loop.exec();
    QByteArray PanFileListInfo = replyPanFileListInfo->readAll();
    //qDebug()<<PanFileListInfo;
    QJsonParseError *error=new QJsonParseError;
    QJsonDocument doc=QJsonDocument::fromJson(PanFileListInfo,error);
    if(error->error==QJsonParseError::NoError)//===1
    {
        ui->panMovieListWidget->clear();
        QJsonObject rootObj= doc.object();
        QJsonArray listArray = rootObj["list"].toArray();
        foreach (QJsonValue fileV, listArray) {
            QJsonObject fileObj = fileV.toObject();
            QVariantMap fileObjMap = fileObj.toVariantMap();
            //qDebug()<<fileObjMap;
            QString isdir = fileObjMap.value("isdir").toString();
            //qDebug()<<isdir;
            if(isdir == "1")//===2
            {

                QString dirPath = fileObjMap.value("path").toString();
                dirPath = dirPath.toUtf8().toPercentEncoding();
                //qDebug()<<dirPath;
                QNetworkReply *replyPanFileListInfo = panManager->get(QNetworkRequest(QUrl(QString("http://pan.baidu.com/api/list?channel=chunlei&clienttype=0&web=1&num=50&page=1&dir=%0&order=time&desc=1&showempty=0&bdstoken=%1&app_id=250528").arg(dirPath).arg(token))));
                QObject::connect(replyPanFileListInfo,SIGNAL(finished()), &loop, SLOT(quit()), Qt::DirectConnection);
                loop.exec();
                QByteArray PanFileListInfo = replyPanFileListInfo->readAll();
                QJsonParseError *error=new QJsonParseError;
                QJsonDocument doc=QJsonDocument::fromJson(PanFileListInfo,error);
                if(error->error==QJsonParseError::NoError)
                {
                    QJsonObject rootObj= doc.object();
                    QJsonArray listArray = rootObj["list"].toArray();
                    foreach (QJsonValue fileV, listArray) {
                        QJsonObject fileObj = fileV.toObject();
                        QVariantMap fileObjMap = fileObj.toVariantMap();
                        QString isdir = fileObjMap.value("isdir").toString();
                        if(isdir == "1")//===3
                        {
                            QString dirPath = fileObjMap.value("path").toString();
                            dirPath = dirPath.toUtf8().toPercentEncoding();
                            QNetworkReply *replyPanFileListInfo = panManager->get(QNetworkRequest(QUrl(QString("http://pan.baidu.com/api/list?channel=chunlei&clienttype=0&web=1&num=50&page=1&dir=%0&order=time&desc=1&bdstoken=%1").arg(dirPath).arg(token))));
                            QObject::connect(replyPanFileListInfo,SIGNAL(finished()), &loop, SLOT(quit()), Qt::DirectConnection);
                            loop.exec();
                            QByteArray PanFileListInfo = replyPanFileListInfo->readAll();
                            QJsonParseError *error=new QJsonParseError;
                            QJsonDocument doc=QJsonDocument::fromJson(PanFileListInfo,error);
                            if(error->error==QJsonParseError::NoError)
                            {
                                QJsonObject rootObj= doc.object();
                                QJsonArray listArray = rootObj["list"].toArray();
                                foreach (QJsonValue fileV, listArray) {
                                    QJsonObject fileObj = fileV.toObject();
                                    QVariantMap fileObjMap = fileObj.toVariantMap();
                                    QString isdir = fileObjMap.value("isdir").toString();
                                    if(isdir == "1")
                                    {

                                    }else
                                    {
                                        QString path = fileObjMap.value("path").toString();
                                        ui->panMovieListWidget->addItem(path);
                                    }
                                }

                            }
                        }else
                        {
                            QString path = fileObjMap.value("path").toString();
                            ui->panMovieListWidget->addItem(path);
                        }
                    }

                }

            }else
            {
                QString path = fileObjMap.value("path").toString();
                ui->panMovieListWidget->addItem(path);
            }

        }
    }
    ui->refreshFilesList->setEnabled(true);
}

void MagnetWidget::on_newUrlTaskBtn_clicked()
{
    bool ok;
    QString UrlText = QInputDialog::getText(this, tr("请输入链接"),
                                               tr("链接地址:"), QLineEdit::Normal,
                                               "Url:", &ok);
    if (ok && !UrlText.isEmpty())
    {
        newUrlTask(UrlText);
    }
}
