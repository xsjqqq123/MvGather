#ifndef BROWSEWIDGET_H
#define BROWSEWIDGET_H
#include <QDebug>
#include <QWidget>
#include <QGridLayout>
#include <QToolButton>
#include <QToolButton>
#include <QButtonGroup>
#include <QStackedLayout>
#include "../ListShowWidget/listshowwidget.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkReply>
#include <QEventLoop>
#include <QLineEdit>
#include "adshow.h"
#include "../mytip.h"
class BrowseWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BrowseWidget(QWidget *parent = 0);
    void initUi();
    bool isFirstLoad;
signals:
    void play(QString tvUrl);
public slots:
    void updateShowWidget();
    void getTuiMvList();
    void sendPlay(QString tvUrl, QString title);
    void search();
private:
    QNetworkAccessManager *manager;
    ListShowWidget *showWidget;//显示视频列表
    QWidget *navPanelTWidget;//导航分类面版，上部分，用于放置五个大分类
    QWidget *navPanelBWidget;//导航分类面版，下部分，用于放置各种小分类
    QGridLayout *mainGLayout;//对本部件整个排列
    QWidget *moPanelWidget;//mo指movie,电视剧tv、动漫ct、综艺va、微电影vm
    QWidget *tvPanelWidget;
    QWidget *ctPanelWidget;
    QWidget *vaPanelWidget;
    QWidget *vmPanelWidget;
    QWidget *tuiPanelWidget;

    QGridLayout *navGLayout;//检索总分类,用于存放 电影、电视剧、动漫、综艺、微电影\主页推荐 按钮
    QStackedLayout *navSLayout;//用于存放 电影、电视剧、动漫、综艺、微电影共五大分类的layout
    QButtonGroup *navBtnGroup;

    QGridLayout *moGLayout;//电影分类
    QButtonGroup *moCatGroup;//电影"类型"按钮分组
    QButtonGroup *moAreaGroup;//电影“地区”分类处按钮分组
    QButtonGroup *moYearGroup;//电影“年代”分类处按钮分组
    QButtonGroup *moRankGroup;//电影“排序方式”分类处按钮分组

    QGridLayout *tvGLayout;//电视剧分类
    QButtonGroup *tvCatGroup;//电视剧"类型"按钮分组
    QButtonGroup *tvAreaGroup;//电视剧“地区”分类处按钮分组
    QButtonGroup *tvYearGroup;//电视剧“年代”分类处按钮分组
    QButtonGroup *tvRankGroup;//电视剧“排序方式”分类处按钮分组

    QGridLayout *ctGLayout;//动漫分类
    QButtonGroup *ctCatGroup;//动漫"类型"按钮分组
    QButtonGroup *ctAreaGroup;//动漫“地区”分类处按钮分组
    QButtonGroup *ctYearGroup;//动漫“年代”分类处按钮分组
    QButtonGroup *ctRankGroup;//动漫“排序方式”分类处按钮分组

    QGridLayout *vaGLayout;//综艺分类
    QButtonGroup *vaCatGroup;//综艺"类型"按钮分组
    QButtonGroup *vaAreaGroup;//综艺“地区”分类处按钮分组
    QButtonGroup *vaYearGroup;//综艺“年代”分类处按钮分组
    QButtonGroup *vaRankGroup;//综艺“排序方式”分类处按钮分组

    QGridLayout *vmGLayout;//微电影分类
    QButtonGroup *vmCatGroup;//微电影"类型"按钮分组
    QButtonGroup *vmAreaGroup;//微电影“地区”分类处按钮分组
    QButtonGroup *vmYearGroup;//微电影“年代”分类处按钮分组
    QButtonGroup *vmRankGroup;//微电影“排序方式”分类处按钮分组

    QGridLayout *tuiGLayout;//主页与搜索
    QButtonGroup *tuiGroup;//推荐按钮分组

    QLineEdit *searchLineEdit;//搜索
    QToolButton *searchBtn;

    AdShow *adShowPanel;
    MyTip myTipWin;
};

#endif // BROWSEWIDGET_H
