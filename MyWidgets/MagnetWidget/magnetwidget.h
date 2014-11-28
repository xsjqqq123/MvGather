#ifndef MAGNETWIDGET_H
#define MAGNETWIDGET_H

#include <QWidget>
#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QEventLoop>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonParseError>
#include <QRegExp>
#include <QTableWidgetItem>
#include <QTimer>
#include <QMenu>
#include <QAction>
#include "loginformwin.h"
#include "vcodedialog.h"
#include "../mytip.h"
namespace Ui {
class MagnetWidget;
}

class MagnetWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MagnetWidget(QWidget *parent = 0);
    ~MagnetWidget();
    bool isFirstLoad;
signals:
    void addDownloadTask(QString url);
public slots:
    void getMp4baMagnetList();
private slots:
    void on_searchLineEdit_returnPressed();

    void getMagnetList(QString keyWord);
    void setPanInfo(QString text);
    void getRealUrl(QString path);
    void newMagnetTask(QString url);
    void newUrlTask(QString UrlText);//
    void on_searchBtn_clicked();

    void on_loginBtn_clicked();

    void loginSuccess(QString userName,QString token);

    void on_panMovieListWidget_doubleClicked(const QModelIndex &index);

    void on_panMovieListWidget_customContextMenuRequested(const QPoint &pos);

    void on_clearTaskBtn_clicked();

    void on_downloadMagnetlistWidget_customContextMenuRequested(const QPoint &pos);

    void on_magnetSearchTbWidget_customContextMenuRequested(const QPoint &pos);


    void on_deleteFiles_clicked();

    void on_newMagnet_clicked();

    void on_refreshTaskBtn_clicked();

    void on_refreshFilesList_clicked();

    void on_newUrlTaskBtn_clicked();

private:
    Ui::MagnetWidget *ui;
    QNetworkAccessManager *searchManager;
    QNetworkAccessManager *panManager;
    LoginFormWin loginFormWin;
    QString token;
    QString userName;
    MyTip myTipWin;
};

#endif // MAGNETWIDGET_H
