#ifndef DOWNLOADWIDGET_H
#define DOWNLOADWIDGET_H

#include <QWidget>
#include <QFile>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include <QTimer>
#include <QSettings>
#include <QProcess>
#include <QSqlQuery>
#include <QRegExp>
#include <QFileInfo>
#include "../mytip.h"
namespace Ui {
class DownloadWidget;
}

class DownloadWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DownloadWidget(QWidget *parent = 0, QString url = "", QString fileSavePath="", qint64 taskId = 0, int percent = 0);
    ~DownloadWidget();
protected:
    void closeEvent(QCloseEvent *);
signals:
    void play(QString filePath);
public slots:
    void startDownload(QString url,QString fileSavePath);
    QString getRealUrl(QString url);
    void readyReadStandardOutput();
    void readyReadStandardError();
    void saveSetting();
    void on_startBtn_clicked();

    void on_playBtn_clicked();

    void on_pauseBtn_clicked();

    void on_deleteBtn_clicked();

private:
    Ui::DownloadWidget *ui;
    QProcess *process;
    QString url;
    QString fileSavePath;
    qint64 taskId;
    double percent;
    QString realUrl;
    QNetworkAccessManager *manager;
    QTimer *saveSettingTimer;
    MyTip myTipWin;

};

#endif // DOWNLOADWIDGET_H
