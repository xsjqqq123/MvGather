#include "browsewidget.h"//
#include <QLabel>
#include <QRegExp>
#include <QTimer>
#include <QMessageBox>

BrowseWidget::BrowseWidget(QWidget *parent) :
    QWidget(parent)
{
    isFirstLoad = true;
    manager = new QNetworkAccessManager;
    initUi();
    //QTimer::singleShot(2000, this, SLOT(updateShowWidget()));
}

void BrowseWidget::initUi()
{
    showWidget = new ListShowWidget();
    connect(showWidget,SIGNAL(pageChanged()),this,SLOT(updateShowWidget()));
    connect(showWidget,SIGNAL(clicked(QString,QString)),this,SLOT(sendPlay(QString,QString)));
    navPanelTWidget=new QWidget();
    navPanelBWidget=new QWidget();
    mainGLayout = new QGridLayout(this);
    navGLayout = new QGridLayout();
    navSLayout = new QStackedLayout();

    navPanelTWidget->setMaximumWidth(400);
    navPanelBWidget->setMaximumWidth(400);

    mainGLayout->setMargin(0);
    mainGLayout->setSpacing(2);

    mainGLayout->addWidget(showWidget,0,0,10,10);
    mainGLayout->addWidget(navPanelTWidget,0,10,1,2);
    mainGLayout->addWidget(navPanelBWidget,1,10,9,2);

    moGLayout = new QGridLayout();
    tvGLayout = new QGridLayout();
    ctGLayout = new QGridLayout();
    vaGLayout = new QGridLayout();
    vmGLayout = new QGridLayout();
    tuiGLayout = new QGridLayout();

    moGLayout->setAlignment(Qt::AlignHCenter|Qt::AlignTop);
    tvGLayout->setAlignment(Qt::AlignHCenter|Qt::AlignTop);
    ctGLayout->setAlignment(Qt::AlignHCenter|Qt::AlignTop);
    vaGLayout->setAlignment(Qt::AlignHCenter|Qt::AlignTop);
    vmGLayout->setAlignment(Qt::AlignHCenter|Qt::AlignTop);
    tuiGLayout->setAlignment(Qt::AlignHCenter|Qt::AlignTop);

    moGLayout->setMargin(2);
    tvGLayout->setMargin(2);
    ctGLayout->setMargin(2);
    vaGLayout->setMargin(2);
    vmGLayout->setMargin(2);
    tuiGLayout->setMargin(2);

    moGLayout->setSpacing(1);
    tvGLayout->setSpacing(1);
    ctGLayout->setSpacing(1);
    vaGLayout->setSpacing(1);
    vmGLayout->setSpacing(1);
    //tuiGLayout->setSpacing(1);


    moPanelWidget = new QWidget();
    tvPanelWidget = new QWidget();
    ctPanelWidget = new QWidget();
    vaPanelWidget = new QWidget();
    vmPanelWidget = new QWidget();
    tuiPanelWidget = new QWidget();

    moPanelWidget->setLayout(moGLayout);
    tvPanelWidget->setLayout(tvGLayout);
    ctPanelWidget->setLayout(ctGLayout);
    vaPanelWidget->setLayout(vaGLayout);
    vmPanelWidget->setLayout(vmGLayout);
    tuiPanelWidget->setLayout(tuiGLayout);
    navPanelTWidget->setLayout(navGLayout);
    navPanelBWidget->setLayout(navSLayout);

    navSLayout->addWidget(moPanelWidget);
    navSLayout->addWidget(tvPanelWidget);
    navSLayout->addWidget(ctPanelWidget);
    navSLayout->addWidget(vaPanelWidget);
    navSLayout->addWidget(vmPanelWidget);
    navSLayout->addWidget(tuiPanelWidget);
    navSLayout->setCurrentIndex(5);
    //大类
    QPushButton *bt0 = new QPushButton;
    QPushButton *bt1 = new QPushButton;
    QPushButton *bt2 = new QPushButton;
    QPushButton *bt3= new QPushButton;
    QPushButton *bt4 = new QPushButton;
    QPushButton *bt5 = new QPushButton;

    bt4->setVisible(false);//暂时无用


    bt0->setCheckable(true);

    bt1->setCheckable(true);
    bt2->setCheckable(true);
    bt3->setCheckable(true);
    bt4->setCheckable(true);
    bt5->setCheckable(true);
    bt5->setChecked(true);
    bt0->setText("电影");
    bt1->setText("电视剧");
    bt2->setText("动漫");
    bt3->setText("综艺");
    bt4->setText("微电影");
    bt5->setText("搜索");
    navGLayout->addWidget(bt0,0,0,1,1);
    navGLayout->addWidget(bt1,0,1,1,1);
    navGLayout->addWidget(bt2,0,2,1,1);
    navGLayout->addWidget(bt3,0,3,1,1);
    navGLayout->addWidget(bt4,0,4,1,1);
    navGLayout->addWidget(bt5,0,5,1,1);
    navBtnGroup = new QButtonGroup;
    connect(navBtnGroup,SIGNAL(buttonClicked(int)),this,SLOT(updateShowWidget()));
    navBtnGroup->setExclusive(true);
    navBtnGroup->addButton(bt0,0);
    navBtnGroup->addButton(bt1,1);
    navBtnGroup->addButton(bt2,2);
    navBtnGroup->addButton(bt3,3);
    navBtnGroup->addButton(bt4,4);
    navBtnGroup->addButton(bt5,5);
    connect(navBtnGroup,SIGNAL(buttonClicked(int)),navSLayout,SLOT(setCurrentIndex(int)));

    //电影大类下的"类型"
    moCatGroup = new QButtonGroup;
    connect(moCatGroup,SIGNAL(buttonClicked(int)),this,SLOT(updateShowWidget()));
    moCatGroup->setExclusive(true);
    QLabel *moCatLbl = new QLabel("类型:");
    moCatLbl->setMaximumHeight(30);
    moGLayout->addWidget(moCatLbl);
    int row=1;
    int column=0;
    int i=0;
    QString str="全部#剧情#爱情#喜剧#动作#恐怖#惊悚#犯罪#悬疑#科幻#伦理#战争#动画#文艺#历史#传记#奇幻#古装#歌舞#纪录#家庭#冒险#灾难#武侠#青春#其他";
    QStringList strList=str.split("#");
    foreach (QString str_temp, strList) {
        QPushButton *moCatBt = new QPushButton;
        moCatBt->setCheckable(true);
        moCatBt->setText(str_temp);
        moGLayout->addWidget(moCatBt,row,column,1,1);
        moCatGroup->addButton(moCatBt,i);
        i++;
        if(row==1&&column==0)
        {
            moCatBt->setChecked(true);
        }
        if(column>4)
        {
            column=0;
            row++;
        }else
        {
            column++;
        }
    }
    //电影大类下的"地区"
    moAreaGroup = new QButtonGroup;
    connect(moAreaGroup,SIGNAL(buttonClicked(int)),this,SLOT(updateShowWidget()));
    moAreaGroup->setExclusive(true);
    QLabel *moareaLbl = new QLabel("地区:");
    moareaLbl->setMaximumHeight(30);
    int StartRow = moGLayout->rowCount();
    row= StartRow;
    column=0;
    i=0;
    moGLayout->addWidget(moareaLbl,row,column,1,1);
    row++;
    str="全部#美国#大陆#香港#韩国#日本#法国#英国#德国#台湾#泰国#印度#其他";
    strList=str.split("#");
    foreach (QString str_temp, strList) {
        QPushButton *moareaBt = new QPushButton;
        moareaBt->setCheckable(true);
        moareaBt->setText(str_temp);
        moGLayout->addWidget(moareaBt,row,column,1,1);
        moAreaGroup->addButton(moareaBt,i);
        i++;
        if(row==(StartRow+1)&&column==0)
        {
            moareaBt->setChecked(true);
        }
        if(column>4)
        {
            column=0;
            row++;
        }else
        {
            column++;
        }
    }
    //电影大类下的"年代"
    moYearGroup = new QButtonGroup;
    connect(moYearGroup,SIGNAL(buttonClicked(int)),this,SLOT(updateShowWidget()));
    moYearGroup->setExclusive(true);
    QLabel *moYearLbl = new QLabel("年代:");
    moYearLbl->setMaximumHeight(30);
    StartRow = moGLayout->rowCount();
    row= StartRow;
    column=0;
    moGLayout->addWidget(moYearLbl,row,column,1,1);
    row++;
    i=0;
    str="全部#2014#2013#2012#2011#2010#2009#2008#2007#2006#更早";
    strList=str.split("#");
    foreach (QString str_temp, strList) {
        QPushButton *moYearBt = new QPushButton;
        moYearBt->setCheckable(true);
        moYearBt->setText(str_temp);
        moGLayout->addWidget(moYearBt,row,column,1,1);
        moYearGroup->addButton(moYearBt,i);
        i++;
        if(row==(StartRow+1)&&column==0)
        {
            moYearBt->setChecked(true);
        }
        if(column>4)
        {
            column=0;
            row++;
        }else
        {
            column++;
        }
    }
    //电影大类下的"排序方式"
    moRankGroup = new QButtonGroup;
    connect(moRankGroup,SIGNAL(buttonClicked(int)),this,SLOT(updateShowWidget()));
    moRankGroup->setExclusive(true);
    QLabel *moRankLbl = new QLabel("年代:");
    moRankLbl->setMaximumHeight(30);
    StartRow = moGLayout->rowCount();
    row= StartRow;
    column=0;
    moGLayout->addWidget(moRankLbl,row,column,1,1);
    row++;
    i=0;
    str="最新#最热#好评#推荐";
    strList=str.split("#");
    foreach (QString str_temp, strList) {
        QPushButton *moRankBt = new QPushButton;
        moRankBt->setCheckable(true);
        moRankBt->setText(str_temp);
        moGLayout->addWidget(moRankBt,row,column,1,1);
        moRankGroup->addButton(moRankBt,i);
        i++;
        if(row==(StartRow+1)&&column==0)
        {
            moRankBt->setChecked(true);
        }
        if(column>4)
        {
            column=0;
            row++;
        }else
        {
            column++;
        }
    }

    //电视剧++++++++++++++++++++++++++++++++++++++++++++++++++++=//
    //电视剧大类下的"类型"
    tvCatGroup = new QButtonGroup;
    connect(tvCatGroup,SIGNAL(buttonClicked(int)),this,SLOT(updateShowWidget()));
    tvCatGroup->setExclusive(true);
    QLabel *tvCatLbl = new QLabel("类型:");
    tvCatLbl->setMaximumHeight(30);
    tvGLayout->addWidget(tvCatLbl);
    row=1;
    column=0;
    i=0;
    str="全部#喜剧#家庭#悬疑#偶像#历史#爱情#警匪#古装#武侠#都市#科幻#励志#谍战#情景#动作#神话#战争#宫廷#农村#其他";
    strList=str.split("#");
    foreach (QString str_temp, strList) {
        QPushButton *tvCatBt = new QPushButton;
        tvCatBt->setCheckable(true);
        tvCatBt->setText(str_temp);
        tvGLayout->addWidget(tvCatBt,row,column,1,1);
        tvCatGroup->addButton(tvCatBt,i);
        i++;
        if(row==1&&column==0)
        {
            tvCatBt->setChecked(true);
        }
        if(column>4)
        {
            column=0;
            row++;
        }else
        {
            column++;
        }
    }
    //电视剧大类下的"地区"
    tvAreaGroup = new QButtonGroup;
    connect(tvAreaGroup,SIGNAL(buttonClicked(int)),this,SLOT(updateShowWidget()));
    tvAreaGroup->setExclusive(true);
    QLabel *tvareaLbl = new QLabel("地区:");
    tvareaLbl->setMaximumHeight(30);
    StartRow = tvGLayout->rowCount();
    row= StartRow;
    column=0;
    tvGLayout->addWidget(tvareaLbl,row,column,1,1);
    row++;
    i=0;
    str="全部#内地#香港#台湾#韩国#泰国#日本#美国#英国#新加坡";
    strList=str.split("#");
    foreach (QString str_temp, strList) {
        QPushButton *tvareaBt = new QPushButton;
        tvareaBt->setCheckable(true);
        tvareaBt->setText(str_temp);
        tvGLayout->addWidget(tvareaBt,row,column,1,1);
        tvAreaGroup->addButton(tvareaBt,i);
        i++;
        if(row==(StartRow+1)&&column==0)
        {
            tvareaBt->setChecked(true);
        }
        if(column>4)
        {
            column=0;
            row++;
        }else
        {
            column++;
        }
    }
    //电视剧大类下的"年代"
    tvYearGroup = new QButtonGroup;
    connect(tvYearGroup,SIGNAL(buttonClicked(int)),this,SLOT(updateShowWidget()));
    tvYearGroup->setExclusive(true);
    QLabel *tvYearLbl = new QLabel("年代:");
    tvYearLbl->setMaximumHeight(30);
    StartRow = tvGLayout->rowCount();
    row= StartRow;
    column=0;
    i=0;
    tvGLayout->addWidget(tvYearLbl,row,column,1,1);
    row++;
    str="全部#2014#2013#2012#2011#2010#2009#2008#2007#2006#更早";
    strList=str.split("#");
    foreach (QString str_temp, strList) {
        QPushButton *tvYearBt = new QPushButton;
        tvYearBt->setCheckable(true);
        tvYearBt->setText(str_temp);
        tvGLayout->addWidget(tvYearBt,row,column,1,1);
        tvYearGroup->addButton(tvYearBt,i);
        i++;
        if(row==(StartRow+1)&&column==0)
        {
            tvYearBt->setChecked(true);
        }
        if(column>4)
        {
            column=0;
            row++;
        }else
        {
            column++;
        }
    }
    //电视剧大类下的"排序方式"
    tvRankGroup = new QButtonGroup;
    connect(tvRankGroup,SIGNAL(buttonClicked(int)),this,SLOT(updateShowWidget()));
    tvRankGroup->setExclusive(true);
    QLabel *tvRankLbl = new QLabel("年代:");
    tvRankLbl->setMaximumHeight(30);
    StartRow = tvGLayout->rowCount();
    row= StartRow;
    column=0;
    i=0;
    tvGLayout->addWidget(tvRankLbl,row,column,1,1);
    row++;
    str="最新#最热";
    strList=str.split("#");
    foreach (QString str_temp, strList) {
        QPushButton *tvRankBt = new QPushButton;
        tvRankBt->setCheckable(true);
        tvRankBt->setText(str_temp);
        tvGLayout->addWidget(tvRankBt,row,column,1,1);
        tvRankGroup->addButton(tvRankBt,i);
        i++;
        if(row==(StartRow+1)&&column==0)
        {
            tvRankBt->setChecked(true);
        }
        if(column>4)
        {
            column=0;
            row++;
        }else
        {
            column++;
        }
    }

    //动漫++++++++++++++++++++++++++++++++++++++++++++++++++++=//
    //动漫大类下的"类型"
    ctCatGroup = new QButtonGroup;
    connect(ctCatGroup,SIGNAL(buttonClicked(int)),this,SLOT(updateShowWidget()));
    ctCatGroup->setExclusive(true);
    QLabel *ctCatLbl = new QLabel("类型:");
    ctCatLbl->setMaximumHeight(30);
    ctGLayout->addWidget(ctCatLbl);
    row=1;
    column=0;
    i=0;
    str="全部#热血#恋爱#美少女#青春#搞笑#校园#剧情#冒险#机战#魔幻#励志#益智#童话#亲子#科幻#萝莉#动作#运动#耽美#战争#社会#后宫#悬疑#竞技#其他";
    strList=str.split("#");
    foreach (QString str_temp, strList) {
        QPushButton *ctCatBt = new QPushButton;
        ctCatBt->setCheckable(true);
        ctCatBt->setText(str_temp);
        ctGLayout->addWidget(ctCatBt,row,column,1,1);
        ctCatGroup->addButton(ctCatBt,i);
        i++;
        if(row==1&&column==0)
        {
            ctCatBt->setChecked(true);
        }
        if(column>4)
        {
            column=0;
            row++;
        }else
        {
            column++;
        }
    }
    //动漫大类下的"地区"
    ctAreaGroup = new QButtonGroup;
    connect(ctAreaGroup,SIGNAL(buttonClicked(int)),this,SLOT(updateShowWidget()));
    ctAreaGroup->setExclusive(true);
    QLabel *ctareaLbl = new QLabel("地区:");
    ctareaLbl->setMaximumHeight(30);
    StartRow = ctGLayout->rowCount();
    row= StartRow;
    column=0;
    i=0;
    ctGLayout->addWidget(ctareaLbl,row,column,1,1);
    row++;
    str="全部#日本#美国#大陆#其他";
    strList=str.split("#");
    foreach (QString str_temp, strList) {
        QPushButton *ctareaBt = new QPushButton;
        ctareaBt->setCheckable(true);
        ctareaBt->setText(str_temp);
        ctGLayout->addWidget(ctareaBt,row,column,1,1);
        ctAreaGroup->addButton(ctareaBt,i);
        i++;
        if(row==(StartRow+1)&&column==0)
        {
            ctareaBt->setChecked(true);
        }
        if(column>4)
        {
            column=0;
            row++;
        }else
        {
            column++;
        }
    }
    //动漫大类下的"年代"
    ctYearGroup = new QButtonGroup;
    connect(ctYearGroup,SIGNAL(buttonClicked(int)),this,SLOT(updateShowWidget()));
    ctYearGroup->setExclusive(true);
    QLabel *ctYearLbl = new QLabel("年代:");
    ctYearLbl->setMaximumHeight(30);
    StartRow = ctGLayout->rowCount();
    row= StartRow;
    column=0;
    i=0;
    ctGLayout->addWidget(ctYearLbl,row,column,1,1);
    row++;
    str="全部#2014#2013#2012#2011#2010#2009#2008#2007#2006#2005#2004#2003#2002#更早";
    strList=str.split("#");
    foreach (QString str_temp, strList) {
        QPushButton *ctYearBt = new QPushButton;
        ctYearBt->setCheckable(true);
        ctYearBt->setText(str_temp);
        ctGLayout->addWidget(ctYearBt,row,column,1,1);
        ctYearGroup->addButton(ctYearBt,i);
        i++;
        if(row==(StartRow+1)&&column==0)
        {
            ctYearBt->setChecked(true);
        }
        if(column>4)
        {
            column=0;
            row++;
        }else
        {
            column++;
        }
    }
    //动漫大类下的"排序方式"
    ctRankGroup = new QButtonGroup;
    connect(ctRankGroup,SIGNAL(buttonClicked(int)),this,SLOT(updateShowWidget()));
    ctRankGroup->setExclusive(true);
    QLabel *ctRankLbl = new QLabel("年代:");
    ctRankLbl->setMaximumHeight(30);
    StartRow = ctGLayout->rowCount();
    row= StartRow;
    column=0;
    i=0;
    ctGLayout->addWidget(ctRankLbl,row,column,1,1);
    row++;
    str="最新#最热";
    strList=str.split("#");
    foreach (QString str_temp, strList) {
        QPushButton *ctRankBt = new QPushButton;
        ctRankBt->setCheckable(true);
        ctRankBt->setText(str_temp);
        ctGLayout->addWidget(ctRankBt,row,column,1,1);
        ctRankGroup->addButton(ctRankBt,i);
        i++;
        if(row==(StartRow+1)&&column==0)
        {
            ctRankBt->setChecked(true);
        }
        if(column>4)
        {
            column=0;
            row++;
        }else
        {
            column++;
        }
    }
    //综艺++++++++++++++++++++++++++++++++++++++++++++++++++++=//
    //综艺大类下的"类型"
    vaCatGroup = new QButtonGroup;
    connect(vaCatGroup,SIGNAL(buttonClicked(int)),this,SLOT(updateShowWidget()));
    vaCatGroup->setExclusive(true);
    QLabel *vaCatLbl = new QLabel("类型:");
    vaCatLbl->setMaximumHeight(30);
    vaGLayout->addWidget(vaCatLbl);
    row=1;
    column=0;
    i=0;
    str="全部#真人秀#选秀#美食#旅游#访谈#体育#纪实#搞笑#时尚#晚会#音乐#歌舞#游戏#生活#脱口秀#情感#播报#社会#教育#财经#曲艺#其他";
    strList=str.split("#");
    foreach (QString str_temp, strList) {
        QPushButton *vaCatBt = new QPushButton;
        vaCatBt->setCheckable(true);
        vaCatBt->setText(str_temp);
        vaGLayout->addWidget(vaCatBt,row,column,1,1);
        vaCatGroup->addButton(vaCatBt,i);
        i++;
        if(row==1&&column==0)
        {
            vaCatBt->setChecked(true);
        }
        if(column>4)
        {
            column=0;
            row++;
        }else
        {
            column++;
        }
    }
    //综艺大类下的"地区"
    vaAreaGroup = new QButtonGroup;
    connect(vaAreaGroup,SIGNAL(buttonClicked(int)),this,SLOT(updateShowWidget()));
    vaAreaGroup->setExclusive(true);
    QLabel *vaareaLbl = new QLabel("地区:");
    vaareaLbl->setMaximumHeight(30);
    StartRow = vaGLayout->rowCount();
    row= StartRow;
    column=0;
    i=0;
    vaGLayout->addWidget(vaareaLbl,row,column,1,1);
    row++;
    str="全部#大陆#香港#台湾#韩国#日本#美国#其他";
    strList=str.split("#");
    foreach (QString str_temp, strList) {
        QPushButton *vaareaBt = new QPushButton;
        vaareaBt->setCheckable(true);
        vaareaBt->setText(str_temp);
        vaGLayout->addWidget(vaareaBt,row,column,1,1);
        vaAreaGroup->addButton(vaareaBt,i);
        i++;
        if(row==(StartRow+1)&&column==0)
        {
            vaareaBt->setChecked(true);
        }
        if(column>4)
        {
            column=0;
            row++;
        }else
        {
            column++;
        }
    }
    //微电影++++++++++++++++++++++++++++++++++++++++++++++++++++=//
    //微电影大类下的"类型"
    vmCatGroup = new QButtonGroup;
    connect(vmCatGroup,SIGNAL(buttonClicked(int)),this,SLOT(updateShowWidget()));
    vmCatGroup->setExclusive(true);
    QLabel *vmCatLbl = new QLabel("类型:");
    vmCatLbl->setMaximumHeight(30);
    vmGLayout->addWidget(vmCatLbl);
    row=1;
    column=0;
    i=0;
    str="全部#爱情#搞笑#创意#人生#动画#科幻#家庭#励志#动作#特殊#摄影#剧情#悬疑#MV#明星#纪录片#青春#公益#战争#广告#其他";
    strList=str.split("#");
    foreach (QString str_temp, strList) {
        QPushButton *vmCatBt = new QPushButton;
        vmCatBt->setCheckable(true);
        vmCatBt->setText(str_temp);
        vmGLayout->addWidget(vmCatBt,row,column,1,1);
        vmCatGroup->addButton(vmCatBt,i);
        i++;
        if(row==1&&column==0)
        {
            vmCatBt->setChecked(true);
        }
        if(column>4)
        {
            column=0;
            row++;
        }else
        {
            column++;
        }
    }
    //微电影大类下的"年代"
    vmYearGroup = new QButtonGroup;
    connect(vmYearGroup,SIGNAL(buttonClicked(int)),this,SLOT(updateShowWidget()));
    vmYearGroup->setExclusive(true);
    QLabel *vmYearLbl = new QLabel("年代:");
    vmYearLbl->setMaximumHeight(30);
    StartRow = vmGLayout->rowCount();
    row= StartRow;
    column=0;
    i=0;
    vmGLayout->addWidget(vmYearLbl,row,column,1,1);
    row++;
    str="全部#2014#2013#2012#2011";
    strList=str.split("#");
    foreach (QString str_temp, strList) {
        QPushButton *vmYearBt = new QPushButton;
        vmYearBt->setCheckable(true);
        vmYearBt->setText(str_temp);
        vmGLayout->addWidget(vmYearBt,row,column,1,1);
        vmYearGroup->addButton(vmYearBt,i);
        i++;
        if(row==(StartRow+1)&&column==0)
        {
            vmYearBt->setChecked(true);
        }
        if(column>4)
        {
            column=0;
            row++;
        }else
        {
            column++;
        }
    }
    //微电影大类下的"排序方式"
    vmRankGroup = new QButtonGroup;
    connect(vmRankGroup,SIGNAL(buttonClicked(int)),this,SLOT(updateShowWidget()));
    vmRankGroup->setExclusive(true);
    QLabel *vmRankLbl = new QLabel("年代:");
    vmRankLbl->setMaximumHeight(30);
    StartRow = vmGLayout->rowCount();
    row= StartRow;
    column=0;
    i=0;
    vmGLayout->addWidget(vmRankLbl,row,column,1,1);
    row++;
    str="最新#最热";
    strList=str.split("#");
    foreach (QString str_temp, strList) {
        QPushButton *vmRankBt = new QPushButton;
        vmRankBt->setCheckable(true);
        vmRankBt->setText(str_temp);
        vmGLayout->addWidget(vmRankBt,row,column,1,1);
        vmRankGroup->addButton(vmRankBt,i);
        i++;
        if(row==(StartRow+1)&&column==0)
        {
            vmRankBt->setChecked(true);
        }
        if(column>4)
        {
            column=0;
            row++;
        }else
        {
            column++;
        }
    }
    //主页推荐++++++++++++++++++++++++++++++++++++++++++++++++++++=//
    tuiGroup = new QButtonGroup;
    connect(tuiGroup,SIGNAL(buttonClicked(int)),this,SLOT(getTuiMvList()));
    tuiGroup->setExclusive(true);
    QLabel *tuiLbl = new QLabel("推荐:");
    tuiLbl->setMaximumHeight(30);
    StartRow = tuiGLayout->rowCount();
    row= StartRow;
    column=0;
    i=0;
    tuiGLayout->addWidget(tuiLbl,row,column,1,1);
    row++;
    str="推荐电影#推荐电视剧#推荐动漫#推荐综艺";
    strList=str.split("#");
    foreach (QString str_temp, strList) {
        QPushButton *tuiBt = new QPushButton;
        tuiBt->setCheckable(true);
        tuiBt->setText(str_temp);
        tuiGLayout->addWidget(tuiBt,row,column,1,1);
        tuiGroup->addButton(tuiBt,i);
        i++;
        if(row==(StartRow+1)&&column==0)
        {
            tuiBt->setChecked(true);
        }
        if(column>4)
        {
            column=0;
            row++;
        }else
        {
            column++;
        }
    }
    QLabel *soLbl = new QLabel("搜索:");
    searchLineEdit =  new QLineEdit;
    searchBtn = new QPushButton;
    searchBtn->setText("搜索");
    tuiGLayout->addWidget(soLbl,row+1,0,1,4);
    tuiGLayout->addWidget(searchLineEdit,row+2,0,1,3);
    tuiGLayout->addWidget(searchBtn,row+2,3,1,1);
    connect(searchBtn,SIGNAL(clicked()),this,SLOT(search()));
    connect(searchLineEdit,SIGNAL(returnPressed()),this,SLOT(search()));

    QLabel *adLbl = new QLabel("公告:");
    adShowPanel =  new AdShow;
    tuiGLayout->addWidget(adLbl,row+3,0,1,4);
    tuiGLayout->addWidget(adShowPanel,row+4,0,1,4);


    //setLayout(mainGLayout);
}

void BrowseWidget::updateShowWidget()//判断选的类型构造链接，并请求相应地址，得到数据，再更新视频列表
{
    QString url="http://www.yunfan.com/list/list_%1.php?showtype=list&list=1&rank=%2&year=%3&area=%4&act=&cat=%6&p=%7";
    url = url.replace("%7",QString::number(showWidget->page));

    switch (navBtnGroup->checkedId()) {//判断
    case 0:
        url = url.replace("%1","mo");
        if(moCatGroup->checkedId()==0)
        {
            url = url.replace("%6","all");
        }else
        {
            url = url.replace("%6",QString::number(moCatGroup->checkedId()));
        }

        switch (moAreaGroup->checkedId()) {
        case 0:
            url = url.replace("%4","all");
            break;
        case 1:
            url = url.replace("%4","11");
            break;
        case 2:
            url = url.replace("%4","10");
            break;
        case 3:
            url = url.replace("%4","15");
            break;
        case 4:
            url = url.replace("%4","13");
            break;
        case 5:
            url = url.replace("%4","14");
            break;
        case 6:
            url = url.replace("%4","12");
            break;
        case 7:
            url = url.replace("%4","16");
            break;
        case 8:
            url = url.replace("%4","17");
            break;
        case 9:
            url = url.replace("%4","18");
            break;

        case 10:
            url = url.replace("%4","21");
            break;
        case 11:
            url = url.replace("%4","22");
            break;
        default:
            url = url.replace("%4","other");
            break;
        }
        if(moYearGroup->checkedButton()->text()=="全部")
        {
            url = url.replace("%3","all");
        }else if(moYearGroup->checkedButton()->text()=="其他")
        {
            url = url.replace("%3","other");
        }else
        {
            url = url.replace("%3",moYearGroup->checkedButton()->text());
        }
        switch (moRankGroup->checkedId()) {
        case 1:
            url = url.replace("%2","rankhot");
            break;
        case 0:
            url = url.replace("%2","createtime");
            break;
        case 2:
            url = url.replace("%2","rankpoint");
            break;
        case 3:
            url = url.replace("%2","recom");
            break;
        default:
            url = url.replace("%2","rankhot");
            break;
        }
        break;
    case 1:
        url = url.replace("%1","tv");
        if(tvCatGroup->checkedId()==0)
        {
            url = url.replace("%6","all");
        }else
        {
            url = url.replace("%6",QString::number(tvCatGroup->checkedId()));
        }
        switch (tvAreaGroup->checkedId()) {
        case 0:
            url = url.replace("%4","all");
            break;
        case 1:
            url = url.replace("%4","10");
            break;
        case 2:
            url = url.replace("%4","11");
            break;
        case 3:
            url = url.replace("%4","16");
            break;
        case 4:
            url = url.replace("%4","12");
            break;
        case 5:
            url = url.replace("%4","14");
            break;
        case 6:
            url = url.replace("%4","15");
            break;
        case 7:
            url = url.replace("%4","17");
            break;
        case 8:
            url = url.replace("%4","13");
            break;
        case 9:
            url = url.replace("%4","18");
            break;
        default:
            url = url.replace("%4","other");
            break;
        }
        if(tvYearGroup->checkedButton()->text()=="全部")
        {
            url = url.replace("%3","all");
        }else if(tvYearGroup->checkedButton()->text()=="更早")
        {
            url = url.replace("%3","other");
        }else
        {
            url = url.replace("%3",tvYearGroup->checkedButton()->text());
        }
        switch (tvRankGroup->checkedId()) {
        case 1:
            url = url.replace("%2","rankhot");
            break;
        case 0:
            url = url.replace("%2","createtime");
            break;
        default:
            url = url.replace("%2","rankhot");
            break;
        }
        break;
    case 2:
        url = url.replace("%1","ct");
        if(ctCatGroup->checkedId()==0)
        {
            url = url.replace("%6","all");
        }else
        {
            url = url.replace("%6",QString::number(ctCatGroup->checkedId()));
        }
        switch (ctAreaGroup->checkedId()) {
        case 0:
            url = url.replace("%4","all");
            break;
        case 1:
            url = url.replace("%4","11");
            break;
        case 2:
            url = url.replace("%4","12");
            break;
        case 3:
            url = url.replace("%4","10");
            break;
        default:
            url = url.replace("%4","other");
            break;
        }
        if(ctYearGroup->checkedButton()->text()=="全部")
        {
            url = url.replace("%3","all");
        }else if(ctYearGroup->checkedButton()->text()=="更早")
        {
            url = url.replace("%3","other");
        }else
        {
            url = url.replace("%3",ctYearGroup->checkedButton()->text());
        }
        switch (ctRankGroup->checkedId()) {
        case 1:
            url = url.replace("%2","rankhot");
            break;
        case 0:
            url = url.replace("%2","pub");
            break;
        default:
            url = url.replace("%2","rankhot");
            break;
        }

        break;
    case 3:
        url ="http://www.yunfan.com/list/list_%1.php?cat=%6&area=%4";
        url = url.replace("%1","va");
        if(vaCatGroup->checkedId()==0)
        {
            url = url.replace("%6","all");
        }else
        {
            url = url.replace("%6",QString::number(vaCatGroup->checkedId()));
        }
        switch (vaAreaGroup->checkedId()) {
        case 0:
            url = url.replace("%4","all");
            break;
        case 1:
            url = url.replace("%4","1");
            break;
        case 2:
            url = url.replace("%4","2");
            break;
        case 3:
            url = url.replace("%4","3");
            break;
        case 4:
            url = url.replace("%4","4");
            break;
        case 5:
            url = url.replace("%4","5");
            break;
        case 6:
            url = url.replace("%4","6");
            break;
        default:
            url = url.replace("%4","7");
            break;
        }

        break;
    case 4:
        url = url.replace("%1","vm");
        if(vaCatGroup->checkedId()==0)
        {
            url = url.replace("%6","all");
        }else
        {
            url = url.replace("%6",QString::number(vmCatGroup->checkedId()));
        }
        if(vmYearGroup->checkedButton()->text()=="全部")
        {
            url = url.replace("%3","all");
        }else
        {
            url = url.replace("%3",vmYearGroup->checkedButton()->text());
        }
        switch (vmRankGroup->checkedId()) {
        case 1:
            url = url.replace("%2","rankhot");
            break;
        case 0:
            url = url.replace("%2","createtime");
            break;
        default:
            url = url.replace("%2","rankhot");
            break;
        }
        break;
    case 5:
        getTuiMvList();
        return;
        break;
    default:
        break;
    }

    myTipWin.show();
    myTipWin.setBusyIndicatorShow(true);
    myTipWin.setText("正在请求数据");

    QNetworkReply *reply = manager->get(QNetworkRequest(QUrl(url)));
    QEventLoop loop;
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
    QByteArray data = reply->readAll();
    //解析结果得列表
    QStringList list;
    QString str_all;
    QStringList str_ones;
    QRegExp rx_all("<ul id=\"listcontent\">.*</ul>");
    QRegExp rx_ones("<li>.*</li>");
    rx_all.setMinimal(true);
    rx_ones.setMinimal(true);
    int pos = 0;
    while ((pos = rx_all.indexIn(data, pos)) != -1) {//得到所有数据
        str_all.append(rx_all.cap(0));
        pos += rx_all.matchedLength();
    }

    pos = 0;
    while ((pos = rx_ones.indexIn(str_all, pos)) != -1) {//得到单条数据
        str_ones.append(rx_ones.cap(0));
        pos += rx_ones.matchedLength();
    }
    QRegExp tvUrl_rx("<a href=.*html");
    QRegExp imgUrl_rx("data-original=.*&t=p");
    QRegExp title_rx("<p class=.*</p>");
    QRegExp des_rx("<p>.*</p>");
    tvUrl_rx.setMinimal(true);
    imgUrl_rx.setMinimal(true);
    title_rx.setMinimal(true);
    des_rx.setMinimal(true);

    foreach (QString str_one, str_ones) {
        QString tvUrl,imgUrl,title,descripe;
        pos = 0;
        tvUrl.append("http://www.yunfan.com");
        QString t1;
        while ((pos = tvUrl_rx.indexIn(str_one, pos)) != -1) {//l单条数据得到tvUr
            t1=tvUrl_rx.cap(0);
            pos += tvUrl_rx.matchedLength();

        }
        tvUrl.append(t1.replace(QRegExp("<a href=|\'|\""),""));

        pos = 0;
        QString t2;
        while ((pos = imgUrl_rx.indexIn(str_one, pos)) != -1) {//得到单条数据
            t2=imgUrl_rx.cap(0);
            pos += imgUrl_rx.matchedLength();
        }
        imgUrl = t2.replace(QRegExp("data-original=|\'|\""),"");

        pos = 0;
        QString t3;
        while ((pos = title_rx.indexIn(str_one, pos)) != -1) {//得到单条数据
            t3=title_rx.cap(0);
            pos += title_rx.matchedLength();
        }
        QRegExp temp_rx("<.*>");
        temp_rx.setMinimal(true);
        title = t3.replace(temp_rx,"");

        pos = 0;
        QStringList t4;
        while ((pos = des_rx.indexIn(str_one, pos)) != -1) {//得到单条数据
            t4.append(des_rx.cap(0).replace(temp_rx,""));
            pos += des_rx.matchedLength();
        }
        descripe = t4.value(2);//.replace("&middot;","");
        //imgUrl.replace("middle","small");
        list<<(tvUrl+"#"+imgUrl+"#"+title+"#"+descripe);
    }
    //list<<"http://www.yunfan.com/show/mo/6955431352373450634.html#http://img.yunfan.com/index.php?mid=middle_6955431352373450634&t=p#肖申克的救赎#主演：蒂姆·罗宾斯/摩根·弗";

    showWidget->loadDataToList(list);
    showWidget->showPage();
    myTipWin.setText("加载完毕");
    myTipWin.timeToHide(2);

}

void BrowseWidget::getTuiMvList()
{
    myTipWin.show();
    myTipWin.setBusyIndicatorShow(true);
    myTipWin.setText("正在请求数据");
    QString url;
    switch (tuiGroup->checkedId()) {//判断
    case 0:
        url="http://dy.yunfan.com/";
        break;
    case 1:
        url="http://ds.yunfan.com/";
        break;
    case 2:
        url="http://dm.yunfan.com/";
        break;
    case 3:
        url="http://zy.yunfan.com/";
        break;
    default:
        url="http://dy.yunfan.com/";
    }
    QNetworkReply *reply = manager->get(QNetworkRequest(QUrl(url)));
    QEventLoop loop;
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
    QString data = QString(reply->readAll());
    QStringList list;
    QStringList dyAllList = data.split("<li>");
    foreach (QString dyOneList, dyAllList) {
        QString tvUrl;
        QString imgUrl;
        QString title;
        QString descripe;

        int pos = 0;
        QString t1;
        QRegExp tvUrl_rx("href=\".*\"");
        tvUrl_rx.setMinimal(true);
        while ((pos = tvUrl_rx.indexIn(dyOneList, pos)) != -1) {//l单条数据得到tvUrl
            t1=tvUrl_rx.cap(0);
            pos += tvUrl_rx.matchedLength();

        }
        tvUrl.append(t1.replace(QRegExp("href=\"|\""),""));

        QString t2;
        pos = 0;
        QRegExp imgUrl_rx("data-original=\".*\"");
        imgUrl_rx.setMinimal(true);
        while ((pos = imgUrl_rx.indexIn(dyOneList, pos)) != -1) {//l单条数据得到imgUrl
            t2=imgUrl_rx.cap(0);
            pos += imgUrl_rx.matchedLength();
        }
        imgUrl.append(t2.replace(QRegExp("data-original=\"|\""),""));

        pos = 0;
        QString t3;
        QRegExp title_rx("<b>.*</b>");
        title_rx.setMinimal(true);
        while ((pos = title_rx.indexIn(dyOneList, pos)) != -1) {//l单条数据得到tvUrl
            t3=title_rx.cap(0);
            pos += title_rx.matchedLength();
        }
        title.append(t3.replace(QRegExp("<b>|</b>"),""));
        pos = 0;
        QString t4;
        QRegExp descripe_rx("mv_sm\".*</p>");
        descripe_rx.setMinimal(true);
        while ((pos = descripe_rx.indexIn(dyOneList, pos)) != -1) {//l单条数据得到descripe
            t4=descripe_rx.cap(0);
            pos += descripe_rx.matchedLength();
        }
        QRegExp descripe_temp("mv_sm\".*>|</p>");
        descripe_temp.setMinimal(true);
        t4.replace(descripe_temp,"");
        descripe.append(t4);
        if((tvUrl=="") | (imgUrl=="") | (title=="") | (descripe==""))
        {
            continue;
        }else
        {
            list<<(tvUrl+"#"+imgUrl+"#"+title+"#"+descripe);
        }

    }
    showWidget->loadDataToList(list);
    myTipWin.setText("加载完毕");
    myTipWin.timeToHide(2);
    //showWidget->showPage();

}

void BrowseWidget::sendPlay(QString tvUrl,QString title)
{
    emit play(tvUrl);
}

void BrowseWidget::search()
{
    myTipWin.show();
    myTipWin.setText("正在搜索");
    myTipWin.setBusyIndicatorShow(true);

    if(searchLineEdit->text().trimmed()=="")
    {
        searchLineEdit->setText("在此填入文字!");
        searchLineEdit->selectAll();
        searchLineEdit->setFocus();
        myTipWin.setText("填入文字");
        myTipWin.timeToHide(2);
        return;
    }
    QString txt=searchLineEdit->text();
    QNetworkReply *reply = manager->get(QNetworkRequest(QUrl(QString("http://www.yunfan.com/s.php?q=%1&p=1").arg(txt))));
    QEventLoop loop;
    QTimer::singleShot(10000,&loop,SLOT(quit()));
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
    QByteArray data = reply->readAll();
    QRegExp rx_list("data-id=.*<div class=\"bt_js\">");
    rx_list.setMinimal(true);
    int pos=0;
    QStringList listTemp;
    while ((pos = rx_list.indexIn(data, pos)) != -1) {//得到数据列表
        listTemp.append(rx_list.cap(0));
        pos += rx_list.matchedLength();
    }
    QStringList args;////数据格式以#分隔,tvUrl#imgUrl#title#descripe
    foreach (QString str, listTemp) {
        QString tvUrl;
        QString imgUrl;
        QString title;
        QString descripe;

        QRegExp rx_tvUrl("/show/.*html");//tvUrl
        rx_tvUrl.setMinimal(true);
        pos=0;
        while ((pos = rx_tvUrl.indexIn(str, pos)) != -1) {
            tvUrl = rx_tvUrl.cap(0);
            pos += rx_tvUrl.matchedLength();
        }
        tvUrl = "http://www.yunfan.com"+tvUrl;

        QRegExp rx_imgUrl("data-original=\".*\"");//imgUrl
        rx_imgUrl.setMinimal(true);
        pos=0;
        while ((pos = rx_imgUrl.indexIn(str, pos)) != -1) {
            imgUrl = rx_imgUrl.cap(0).replace(QRegExp("data-original=\"|\""),"");
            imgUrl = imgUrl.replace("small","middle");
            pos += rx_imgUrl.matchedLength();
        }

        QRegExp rx_title("<div class=\"bt_t width520\">.*</a>");//title
        rx_title.setMinimal(true);
        pos=0;
        while ((pos = rx_title.indexIn(str, pos)) != -1) {
            QRegExp rx("<.*>");
            rx.setMinimal(true);
            title = rx_title.cap(0).replace(rx,"");
            title.trimmed();
            pos += rx_title.matchedLength();
        }
        descripe=" ";
        args<<tvUrl+"#"+imgUrl+"#"+title+"#"+descripe;
    }
    if(args.isEmpty())
    {
//        QMessageBox msgBox;
//        msgBox.setText("毫无结果，请更换关键词");
//        msgBox.exec();
        myTipWin.show();
        myTipWin.setText("毫无结果.");
        myTipWin.timeToHide(3);

    }else
    {
        showWidget->loadDataToList(args);
        myTipWin.setText("搜索完成");
        myTipWin.timeToHide(2);
    }

}
