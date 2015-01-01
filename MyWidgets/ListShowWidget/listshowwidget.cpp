#include "listshowwidget.h"
#include "qmath.h"
#include <QDebug>
#include <QScrollBar>
#include <QSizePolicy>
#include <QRect>
ListShowWidget::ListShowWidget(QWidget *parent) :
    QScrollArea(parent)
{
    setMinimumWidth(200);
    areaWidgetLayout= new QGridLayout;
    areaWidget= new QWidget;
    QSizePolicy policy;
    policy.setHorizontalPolicy(QSizePolicy::Preferred);
    areaWidget->setSizePolicy(policy);
    areaWidget->setLayout(areaWidgetLayout);
    setWidget(areaWidget);

    page=1;
    checkSeeImgTimer = new QTimer;
    connect(checkSeeImgTimer,SIGNAL(timeout()),this,SLOT(loadSeeImg()));
    checkSeeImgTimer->setInterval(1000);
    checkSeeImgTimer->start();

    this->setWidgetResizable(true);

    setObjectName("listScroll");
    setStyleSheet("#listScroll{background: transparent;border:none;border-right:1px solid rgba(255,255,255,30)}");
    areaWidget->setStyleSheet("background: transparent;border:none;");
}

void ListShowWidget::pagePlus()
{
    page++;
    emit pageChanged();
}

void ListShowWidget::pageMinus()
{
    if(page>1)
    {
        page--;
        emit pageChanged();
    }

}

void ListShowWidget::showPage()
{
    preBtn = new QPushButton;
    nextBtn = new QPushButton;
    preBtn->setText("上一页");
    nextBtn->setText("下一页");
    int br=areaWidgetLayout->rowCount();
    areaWidgetLayout->addWidget(preBtn,br+1,0,1,1);
    areaWidgetLayout->addWidget(nextBtn,br+1,1,1,1);
    connect(preBtn,SIGNAL(clicked()),this,SLOT(pageMinus()));
    connect(nextBtn,SIGNAL(clicked()),this,SLOT(pagePlus()));

}

void ListShowWidget::sendclicked(QString tvUrl, QString title)
{
    emit clicked(tvUrl,title);

}

void ListShowWidget::loadSeeImg()
{
    if(areaWidgetLayout->count()<1)
    {
        return;
    }
    int verticalScrollBarValue=0;
    if(this->verticalScrollBar())
    {
        verticalScrollBarValue=this->verticalScrollBar()->value();
    }
    QRect rec= areaWidgetLayout->cellRect(0,0);
    int cellHeight  = rec.height();
    int seeTotalHeight = verticalScrollBarValue+this->viewport()->height();

    if(cellHeight<10)
    {
        cellHeight=225;
    }
    int seeRowCount = qFloor(seeTotalHeight/cellHeight)+1;
    int count;

    if(areaWidgetLayout->count()>seeRowCount*areaWidgetLayout->columnCount())
    {
        count=seeRowCount*areaWidgetLayout->columnCount();
    }else
    {
        count=areaWidgetLayout->count();
    }
    for(int i=0;i<count;i++)
    {
        if(qobject_cast<ListShowWidgetItem *>(areaWidgetLayout->itemAt(i)->widget()))
        {
            ListShowWidgetItem* item = qobject_cast<ListShowWidgetItem *>(areaWidgetLayout->itemAt(i)->widget());
            if(!item->ImgLbl->isImgLoaded)
            {
                item->ImgLbl->getImage();
                item->ImgLbl->isImgLoaded =true;
            }
        }
    }

}

void ListShowWidget::loadDataToList(QStringList args)//传入数据并重新排列
{
    //item 120*225
    int viewWith = this->viewport()->width();
    while(areaWidgetLayout->count() > 0)
    {
        QWidget* widget = areaWidgetLayout->itemAt(0)->widget();
        areaWidgetLayout->removeWidget(widget);
        delete widget;
    }
    //qDebug()<<areaWidget->width();

    //qDebug()<<"view width"<<viewWith;
//    int viewWith = this->width()-this->verticalScrollBar()->width();
    int columnMax=0;
    if(viewWith >500){
        columnMax=qFloor(viewWith/125)-1;
    }else
    {
        columnMax=4;
    }

    int row=0;
    int column=0;
    int timeToDelay=0;//time表示推迟创建线程加载图片的时间
    foreach (QString arg, args) {
        //数据格式以#分隔,tvUrl#imgUrl#title#descripe
        QStringList list_temp =  arg.split('#');
        QString tvUrl= list_temp.value(0,"");
        QString imgUrl= list_temp.value(1,"");
        QString title= list_temp.value(2,"");
        QString descripe= list_temp.value(3,"");

        Item =new ListShowWidgetItem(0,tvUrl,imgUrl,title,descripe,timeToDelay);
        //Item->setStyleSheet("background: black;border:none;");
        timeToDelay+=300;
        connect(Item,SIGNAL(clicked(QString,QString)),this,SLOT(sendclicked(QString,QString)));

        areaWidgetLayout->addWidget(Item,row,column,1,1);

        if(viewWith<500)
        {
            if(column>0)
            {
                column=0;
                row++;
            }else
            {
                column++;
            }
        }else
        {
            if(column>columnMax-1)
            {
                column=0;
                row++;
            }else
            {
                column++;
            }
        }
    }
    if(viewWith<=500)
    {
        int heightMax=(qFloor(areaWidgetLayout->count()/(columnMax))+1+0.3)*225;
        areaWidget->resize(500,heightMax);
    }else
    {
        int heightMax=(qFloor(areaWidgetLayout->count()/(columnMax+1))+1+0.3)*225;
        areaWidget->resize(viewWith,heightMax);
    }
    this->verticalScrollBar()->setValue(0);
}
