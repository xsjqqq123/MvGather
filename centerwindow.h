#ifndef CENTERWINDOW_H
#define CENTERWINDOW_H

#include <QToolButton>
#include <QSqlDatabase>
#include <QKeyEvent>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkReply>
#include <QEventLoop>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include "QFramer/fcenterwindow.h"
#include "MyWidgets/PlayerWidget/playerwidget.h"
#include "MyWidgets/BrowseWidget/browsewidget.h"
#include "MyWidgets/RecommendWidget/recommendwidget.h"
#include "MyWidgets/MagnetWidget/magnetwidget.h"
#include "MyWidgets/ThreadDownload/downloadwidget.h"
class CenterWindow : public FCenterWindow
{
    Q_OBJECT
public:
    explicit CenterWindow(QWidget *parent = 0);
     PlayerWidget *playerWidget;
signals:
    void getIntofullScreenSignal(bool b);
    void getIntoWideSignal(bool b);
public slots:
    void addMvToPlaylist(QString tvUrl);
    void getIntofullScreenMode(bool b);
    void getIntoWideModel(bool b);
    void switchToPlayerAndPlay(QString fileSavePath);
    void checkVersion();
    void timeToCheckV();
    void play(QString fileSavePath);
    void firstLoadList(int i);
    void addDownloadTask(QString url);
    void loadDownloadSettings();
    void hideMouse();
    void setIndex0();

private:

    BrowseWidget *browseWidget;
    RecommendWidget *recommendWidget;
    MagnetWidget *magnetWidget;
    QScrollArea *downloadManageWidget;
    DownloadWidget *downLoadWidget;
    QString version;
    QWidget *downloadManageScrollAreaWidget;
    QVBoxLayout *downloadManageScrollAreaWidgetMainLayout;
    bool isDownLoadManagerFirstLoad;
    QTimer *hideMouseTimer;
    MyTip myTipWin;

};

#endif // CENTERWINDOW_H
