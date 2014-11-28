#include "downloadwidget.h"
#include "ui_downloadwidget.h"
#include <QDebug>
#include <QDir>
DownloadWidget::DownloadWidget(QWidget *parent , QString url, QString fileSavePath, qint64 taskId,int percent) :
    QWidget(parent),
    ui(new Ui::DownloadWidget)
{
    ui->setupUi(this);
    process = new QProcess(this);
    connect(process,SIGNAL(readyReadStandardOutput()),this,SLOT(readyReadStandardOutput()));
    connect(process,SIGNAL(readyReadStandardError()),this,SLOT(readyReadStandardError()));
    manager = new QNetworkAccessManager(this);
    saveSettingTimer = new QTimer(this);
    connect(saveSettingTimer,SIGNAL(timeout()),this,SLOT(saveSetting()));

    this->url = url;
    this->fileSavePath = fileSavePath;
    this->taskId = taskId;
    this->percent = percent;
    ui->startBtn->setEnabled(percent==100?false:true);
    ui->pauseBtn->setEnabled(false);
    ui->playBtn->setEnabled(percent>2?true:false);
    ui->fileNameLbl->setText(QFileInfo(fileSavePath).fileName());
    ui->progressBar->setValue(percent);
    ui->deleteBtn->setIcon(QIcon(QPixmap("://skin/icons/dark/appbar.page.delete.png")));
}

DownloadWidget::~DownloadWidget()
{
    process->terminate();
    process->waitForFinished();
    delete ui;
}

void DownloadWidget::closeEvent(QCloseEvent *)
{

}

void DownloadWidget::startDownload(QString url, QString fileSavePath)
{
    myTipWin.show();
    myTipWin.setBusyIndicatorShow(true);
    myTipWin.setText("获取信息中");
    this->realUrl = getRealUrl(url);
    QFileInfo finfo(fileSavePath);
    QDir dir;
    dir.mkpath(finfo.dir().path());
    process->start(QString("axel -n 5 -o \"%0\" \"%1\"").arg(fileSavePath).arg(this->realUrl));
    saveSettingTimer->start(10*1000);
    //qDebug()<<this->realUrl<<fileSavePath;
    myTipWin.setText("开始下载");
    myTipWin.timeToHide(1);
}

QString DownloadWidget::getRealUrl(QString url)
{
    QString realUrl;
    if(url.contains("plyz"))
    {
        int tryTimes=5;
        //尝试tryTimes次
        QString realUrl_temp = this->url;
        while(tryTimes --)
        {
            QEventLoop loop;
            QNetworkReply *reply = manager->head(QNetworkRequest(realUrl_temp));
            connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
            loop.exec();
            QVariant varLocation = reply->header(QNetworkRequest::LocationHeader);
            if(!varLocation.isNull())
            {
                realUrl_temp = varLocation.toString();
                //qDebug()<<realUrl_temp;
                continue;
            }
            delete reply;
            break;
        }
        realUrl = realUrl_temp;
        if(realUrl == "")
        {
            myTipWin.show();
            myTipWin.setText("获取下载链接失败。");
            myTipWin.timeToHide(3);
        }

    }else if(url.contains("baidupcs.com"))
    {
        realUrl= url;
    }else
    {
        QNetworkRequest request0;
        request0.setUrl(QString("http://www.mvgather.com/mvgather/api/getRealUrl.php?url=%0").arg(url));//
        QNetworkReply *reply0 = manager->get(request0);
        QEventLoop loop0;
        QObject::connect(reply0, SIGNAL(finished()), &loop0, SLOT(quit()));
        loop0.exec();
        QByteArray data0 = reply0->readAll();
        realUrl = data0;
    }
    return realUrl;
}

void DownloadWidget::readyReadStandardOutput()
{
    QString data = process->readAll();
    if(!data.contains("["))
    {
        return;
    }
    //qDebug()<<"data:"<<data;

    QRegExp rx_speed("\\[.*KB/s\\]");
    QRegExp rx_percent("\\d{1,3}%");
    rx_speed.setMinimal(true);
    rx_percent.setMinimal(true);
    if(rx_speed.indexIn(data) != -1)
    {
        QString str = rx_speed.cap(0);
        ui->speedLbl->setText(str.replace(" ",""));
    }
    if(rx_percent.indexIn(data) != -1)
    {
        QString str = rx_percent.cap(0);
        int v = str.replace("%","").toInt();
        ui->progressBar->setValue(v);
        this->percent = v;
        if(v == 100)
        {
            ui->pauseBtn->setEnabled(false);
            ui->speedLbl->setText("[0KB/s]");
        }
        //qDebug()<<"str"<<v;
    }

}

void DownloadWidget::readyReadStandardError()
{
    qDebug()<< process->readAllStandardError();
}

void DownloadWidget::saveSetting()
{
    QSqlQuery query_update;
    query_update.exec(QString("update dtaskTB SET percent = '%0' WHERE taskId = '%1'").arg(this->percent).arg(this->taskId));
    if(ui->progressBar->value()>2)
    {
        ui->playBtn->setEnabled(true);
    }
}

void DownloadWidget::on_startBtn_clicked()
{
    ui->startBtn->setEnabled(false);
    ui->pauseBtn->setEnabled(true);
    startDownload(this->url,this->fileSavePath);
}

void DownloadWidget::on_playBtn_clicked()
{
    emit play(this->fileSavePath);
}

void DownloadWidget::on_pauseBtn_clicked()
{
    if(process->state() == QProcess::Running)
    {
        ui->speedLbl->setText("[0KB/s]");
        process->terminate();
        ui->pauseBtn->setText("继续");
        saveSettingTimer->stop();
    }else if(process->state() == QProcess::NotRunning)
    {
        process->start(QString("axel -n 5 -o \"%0\" \"%1\"").arg(fileSavePath).arg(this->realUrl));
        ui->pauseBtn->setText("暂停");
        saveSettingTimer->start(10*1000);
    }
}

void DownloadWidget::on_deleteBtn_clicked()
{
    QSqlQuery query_delete(QString("delete from dtaskTB where taskId='%0'").arg(taskId));
    query_delete.exec();
    process->terminate();
    this->hide();

    //删除相关文件
    /*
    QFile file;
    QFileInfo finfo(fileSavePath);
    QString dirName = finfo.absolutePath();

    file.setFileName(fileSavePath);
    file.remove();
    file.setFileName(fileSavePath+".st");
    file.remove();

    QDir dir;
    dir.rmdir(dirName);*/
}
