#include "playerlistwidgetitem.h"
#include <QLabel>
#include <QSqlQuery>
#include <QDebug>
PlayerListWidgetItem::PlayerListWidgetItem(QWidget *parent, QString tvId, QString tvName) :
    QWidget(parent)
{
    mainGLayout = new QGridLayout;
    //mainGLayout->setAlignment(Qt::AlignHCenter);//
    setLayout(mainGLayout);
    hLayout = new QHBoxLayout;
    mainGLayout->addLayout(hLayout,0,0,1,5);

    hLayout->setMargin(0);
    hLayout->setSpacing(0);
    mainGLayout->setMargin(0);//
    //mainGLayout->setSpacing(0);

    deleteBtn = new ImgBtn(0,"://skin/icons/dark/appbar.page.delete.png");
    QLabel *tvNameLbl= new QLabel(tvName);
    tvNameLbl->setMaximumWidth(180);
    tvNameLbl->setToolTip(tvName);
    hLayout->addWidget(deleteBtn);
    hLayout->addWidget(tvNameLbl);

    deleteBtn->setMaximumSize(30,30);
    connect(deleteBtn,SIGNAL(clicked()),this,SLOT(sendUpdatePlayList()));

    //tvNameLbl->setMaximumSize(230,30);


    row = 1;
    column =0;
    serialBtnGroup = new QButtonGroup;
    serialBtnGroup->setExclusive(true);
    connect(serialBtnGroup,SIGNAL(buttonClicked(int)),this,SLOT(play(int)));
    connect(serialBtnGroup,SIGNAL(buttonClicked(int)),this,SLOT(updateHistoryNo(int)));

    this->tvId = tvId;
}

void PlayerListWidgetItem::addSerial(QString tvNo, QString historyNo)
{

    serialBtn = new QToolButton;
    //serialBtn->setFlat(true);
    serialBtn->setMaximumWidth(36);
    serialBtn->setMinimumWidth(34);
    serialBtn->setText(tvNo);
    serialBtn->resize(50,30);
    serialBtn->setCheckable(true);
    if(historyNo==tvNo)
    {
        serialBtn->setChecked(true);
    }
    serialBtnGroup->addButton(serialBtn,tvNo.toInt());


    mainGLayout->addWidget(serialBtn,row,column,1,1);
    if(column>3)
    {
        row++;
        column=0;
    }else
    {
        column++;
    }
}

void PlayerListWidgetItem::play(int btNo)
{
    //QString tvNo =  serialBtnGroup->checkedButton()->text();
    emit sendPlay(this->tvId,QString::number(btNo));
}

void PlayerListWidgetItem::updateHistoryNo(int historyNo)
{
    QSqlQuery query_update(QString("update playlistTB set  historyNo='%1' where tvId='%2'").arg(historyNo).arg(this->tvId));
    query_update.exec();
}

int PlayerListWidgetItem::getRow()
{
    return this->row;
}

void PlayerListWidgetItem::sendUpdatePlayList()
{
    QSqlQuery query_delete(QString("DELETE  FROM playlistTB WHERE tvId='%1'").arg(this->tvId));
    query_delete.exec();
    emit updatePlayList();
}
