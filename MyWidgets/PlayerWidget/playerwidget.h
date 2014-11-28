#ifndef PLAYERWIDGET_H
#define PLAYERWIDGET_H

#include <QWidget>
#include <QGridLayout>
#include "playerlistarea.h"
#include "playerwin.h"
#include <QSqlQuery>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QEventLoop>
#include "progressbar.h"
#include "tvlistwidget.h"
#include "analyzer.h"
#include "../mytip.h"
#include <QTimer>
#include <QTabWidget>

class PlayerWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PlayerWidget(QWidget *parent = 0);
    void initUI();
    PlayerWin *playerWin;

signals:
    void hideToFullScreen(bool b);
    void getIntoWideModel(bool b);
public slots:
    void addTvList(QString TvName,QStringList playno_hrefs);//tvname即视频中文名，playno_hrefs，即集数编号与视频网站对应地址，格式为playno#href
    void play(QString tvId,QString tvNO);
    void getRealHref(QString href);
    void addMplayListAndPlay(QString filesUrl, QString quality);
    void SendHideToFullScreen(bool b);
    void SendGetIntoWideModel(bool b);
    void updatePlayList();
    void Player2Ready(int sliceIndex);
    void makePlayer2Ready();
    void writeQuitPro();
    void handlePlayFinished();
    void handleLoadingMedia();
    void PlayNewUrls(QString urlsToPlay,QString quality);
    void loadSlice(int i);
    void addFilesTimer();
    void syncMv();
    void anyliseData(QByteArray data,QString tvId,QString tvUrl,QString source);
    void UpdateAllHrefs();
    void getFlvxzTokens();
    void checkClipboard();
    void playNextOne();
    void seekPos(qreal pos);
    void reset();
private:
    QGridLayout *mainGLayout;
    QTabWidget * PlayerListAreaTab;
    PlayerListArea *listArea;
    TvListWidget *tvListWidget;
    ProgressBar *progressBar;

    QString allHrefs;//
    QString playingHref;//当前正在播放的视频地址
    QString readyHref;//将要播放的视频地址
    int currentSlice;//当前视频分片数
    int totalSlices;//总分片数量
    int nextSliceIndex;//当前视频分片数
    int totalSeries;//剧集总数
    QString quality;//清晰度标记
    QStringList fileList_temp;
    QTimer *timerToUpdateAllHrefs;
    QTimer *clipBoardWatcherTimer;
    QString clipBoardHrefPre;
    QString flvxzTokens;
    QString playingTvId;
    int playingTvNO;
    MyTip *myTipWin;



};

#endif // PLAYERWIDGET_H
