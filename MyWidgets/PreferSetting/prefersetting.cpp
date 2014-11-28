#include "prefersetting.h"
#include <QSettings>
#include <QListWidgetItem>
#include <QInputDialog>
#include <QDesktopWidget>

PreferSetting::PreferSetting(QWidget *parent)
{
    mainLayout =  new QGridLayout(this);
    qualityListWidget =  new QListWidget;
    //qualityListWidget->setMinimumSize(200,300);
    qualityListWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    qualityUpBtn =  new QPushButton("↑");
    qualityDownBtn =  new QPushButton("↓");
    qualityAddBtn =  new QPushButton("添加");
    qualityDeleteBtn =  new QPushButton("删除");
    qualityResetBtn =new QPushButton("重置");
    qualitySaveBtn =  new QPushButton("保存");
    mainLayout->addWidget(qualityListWidget,0,0,5,2);
    mainLayout->addWidget(qualityUpBtn,5,0,1,1);
    mainLayout->addWidget(qualityDownBtn,5,1,1,1);
    mainLayout->addWidget(qualityAddBtn,6,0,1,1);
    mainLayout->addWidget(qualityDeleteBtn,6,1,1,1);
    mainLayout->addWidget(qualityResetBtn,7,0,1,1);
    mainLayout->addWidget(qualitySaveBtn,7,1,1,1);


    connect(qualityUpBtn,SIGNAL(clicked()),this,SLOT(UpQuality()));
    connect(qualityDownBtn,SIGNAL(clicked()),this,SLOT(DownQuality()));
    connect(qualityDeleteBtn,SIGNAL(clicked()),this,SLOT(DeleteQuality()));
    connect(qualityAddBtn,SIGNAL(clicked()),this,SLOT(AddQuality()));
    connect(qualityResetBtn,SIGNAL(clicked()),this,SLOT(ResetQuality()));
    connect(qualitySaveBtn,SIGNAL(clicked()),this,SLOT(saveQuality()));

    QSettings settings("MvGather", "xusongjie");
    preferQualitysSetting = settings.value("app/preferQualitys", "").toString();
    if(preferQualitysSetting =="")
    {
        preferQualitysSetting="高清#超清#M3U8#分段_高清_FLV#分段_高清_MP4#分段_高清_M3U8#分段_720P_FLV#分段_720P_MP4#分段_720P_M3U8#分段_1080P_FLV#分段_1080P_MP4#分段_1080P_M3U8#分段_超清_FLV#分段_超清_MP4#分段_超清_M3U8#分段_标清_FLV#分段_标清_MP4#分段_标清_M3U8#分段_高码1080P_FLV#分段_高码1080P_MP4#分段_高码1080P_M3U8#分段_原画_FLV#分段_原画_MP4#分段_原画_M3U8#分段_4K_FLV#分段_4K_MP4#分段_4K_M3U8#分段_高码4K_FLV#分段_高码4K_MP4#分段_高码4K_M3U8#分段_低清_FLV#分段_低清_MP4#分段_低清_M3U8#单段_高清_MP4#单段_高清_M3U8#单段_高清_FLV#单段_720P_FLV#单段_720P_MP4#单段_720P_M3U8#单段_1080P_FLV#单段_1080P_MP4#单段_1080P_M3U8#单段_超清_FLV#单段_超清_MP4#单段_超清_M3U8#单段_标清_FLV#单段_标清_MP4#单段_标清_M3U8#单段_高码1080P_FLV#单段_高码1080P_MP4#单段_高码1080P_M3U8#单段_原画_FLV#单段_原画_MP4#单段_原画_M3U8#单段_4K_FLV#单段_4K_MP4#单段_4K_M3U8#单段_高码4K_FLV#单段_高码4K_MP4#单段_高码4K_M3U8#单段_低清_FLV#单段_低清_MP4#单段_低清_M3U8";
    }
    qualityListWidget->addItems(preferQualitysSetting.split("#"));
    qualityListWidget->setCurrentRow(0);
    //qualityListWidget->setDragEnabled(true);
    qualityListWidget->setDragDropMode(QAbstractItemView::InternalMove);
    qualityListWidget->setSelectionMode(QAbstractItemView::ContiguousSelection);
}

void PreferSetting::saveQuality()
{
    QStringList list_temp;
    int totalRows = qualityListWidget->count();
    for(int i=0;i<totalRows;i++)
    {
        list_temp.append(qualityListWidget->item(i)->text());
    }
    QSettings settings("MvGather", "xusongjie");
    settings.setValue("app/preferQualitys", list_temp.join("#"));
    emit closeSig();
}

void PreferSetting::UpQuality()
{
    int currentItemIndex = qualityListWidget->currentRow();
    if(currentItemIndex <= 0)
    {
        return;
    }
    QListWidgetItem *currentItem= qualityListWidget->takeItem(qualityListWidget->currentRow());
    qualityListWidget->insertItem(currentItemIndex-1,currentItem);
    qualityListWidget->setCurrentItem(currentItem);
}

void PreferSetting::DownQuality()
{
    int currentItemIndex = qualityListWidget->currentRow();
    if(currentItemIndex >= qualityListWidget->count()-1||currentItemIndex<0 )
    {
        return;
    }
    QListWidgetItem *currentItem= qualityListWidget->takeItem(qualityListWidget->currentRow());
    qualityListWidget->insertItem(currentItemIndex+1,currentItem);
    qualityListWidget->setCurrentItem(currentItem);
}

void PreferSetting::DeleteQuality()
{
    qualityListWidget->takeItem(qualityListWidget->currentRow());
}

void PreferSetting::AddQuality()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("请输入"),
                                         tr("视频清晰度名称:"), QLineEdit::Normal,
                                         "高清", &ok);
    if (ok && !text.isEmpty())
        qualityListWidget->addItem(text);
}

void PreferSetting::ResetQuality()
{
    qualityListWidget->clear();
    preferQualitysSetting="高清#超清#M3U8#分段_高清_FLV#分段_高清_MP4#分段_高清_M3U8#分段_720P_FLV#分段_720P_MP4#分段_720P_M3U8#分段_1080P_FLV#分段_1080P_MP4#分段_1080P_M3U8#分段_超清_FLV#分段_超清_MP4#分段_超清_M3U8#分段_标清_FLV#分段_标清_MP4#分段_标清_M3U8#分段_高码1080P_FLV#分段_高码1080P_MP4#分段_高码1080P_M3U8#分段_原画_FLV#分段_原画_MP4#分段_原画_M3U8#分段_4K_FLV#分段_4K_MP4#分段_4K_M3U8#分段_高码4K_FLV#分段_高码4K_MP4#分段_高码4K_M3U8#分段_低清_FLV#分段_低清_MP4#分段_低清_M3U8#单段_高清_MP4#单段_高清_M3U8#单段_高清_FLV#单段_720P_FLV#单段_720P_MP4#单段_720P_M3U8#单段_1080P_FLV#单段_1080P_MP4#单段_1080P_M3U8#单段_超清_FLV#单段_超清_MP4#单段_超清_M3U8#单段_标清_FLV#单段_标清_MP4#单段_标清_M3U8#单段_高码1080P_FLV#单段_高码1080P_MP4#单段_高码1080P_M3U8#单段_原画_FLV#单段_原画_MP4#单段_原画_M3U8#单段_4K_FLV#单段_4K_MP4#单段_4K_M3U8#单段_高码4K_FLV#单段_高码4K_MP4#单段_高码4K_M3U8#单段_低清_FLV#单段_低清_MP4#单段_低清_M3U8";
    qualityListWidget->addItems(preferQualitysSetting.split("#"));
    qualityListWidget->setCurrentRow(0);
}
