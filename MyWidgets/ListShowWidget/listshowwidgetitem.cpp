#include "listshowwidgetitem.h"
#include <QTimer>
#include <QPainter>
ListShowWidgetItem::ListShowWidgetItem(QWidget *parent, QString tvUrl, QString imgUrl, QString title, QString descripe,int timeToDelay)
{
    this->tvUrl = tvUrl;//对于百度视频，tvUrl代表其ID
    this->imgUrl = imgUrl;
    this->title = title;
    this->descripe = descripe;

    mainGLayout = new QGridLayout(this);
    ImgLbl = new ImageLabel(this,imgUrl,timeToDelay);
    connect(ImgLbl,SIGNAL(clicked()),this,SLOT(SendClicked()));
    titleLbl = new QLabel(title,this);
    descripeLbl = new QLabel("<font color=#333333>"+descripe+"</font>",this);//"<font color=#CCCCCC>"+descripe+"</font>"
    titleLbl->setToolTip(title);
    descripeLbl->setToolTip(descripe);
    //descripeLbl->setStyleSheet("");

    mainGLayout->setAlignment(Qt::AlignVCenter|Qt::AlignHCenter);
    mainGLayout->setSpacing(2);
    mainGLayout->setMargin(4);

    mainGLayout->addWidget(ImgLbl,0,0,6,1);
    mainGLayout->addWidget(titleLbl,6,0,1,1);
    mainGLayout->addWidget(descripeLbl,7,0,1,1);

//    setMaximumSize(210,350);
//    setMinimumSize(190,340);

    setMaximumSize(130,230);
    setMinimumSize(120,220);

}/*

void ListShowWidgetItem::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setBrush(QBrush(QColor(255,255,255,10)));
    painter.setPen(QColor(0,0,0,0));
    painter.drawRect(this->rect());
}*/

void ListShowWidgetItem::SendClicked()
{

    //titleLbl->setStyleSheet("background-color:#7B7B7B;radius:3px");
    ImgLbl->setStyleSheet("border:3px solid #7B7B7B");
    QTimer::singleShot(300,this,SLOT(backgroundColorChange()));
    emit clicked(this->tvUrl,this->title);

}

void ListShowWidgetItem::loadImg()
{
    ImgLbl->getImage();
}

void ListShowWidgetItem::backgroundColorChange()
{
    //titleLbl->setStyleSheet("");
    ImgLbl->setStyleSheet("");
}
