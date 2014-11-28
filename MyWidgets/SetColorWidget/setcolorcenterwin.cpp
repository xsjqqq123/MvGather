#include "setcolorcenterwin.h"
#include "ui_setcolorcenterwin.h"
#include <QDebug>
SetColorCenterWin::SetColorCenterWin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SetColorCenterWin)
{
    ui->setupUi(this);
    QSettings settings("MvGather","xusongjie");
    backColor = settings.value("app/backgroundColor","0,112,132").toByteArray();
    btnColor = settings.value("app/btnColor","0,82,112").toByteArray();
    initUi();
}

SetColorCenterWin::~SetColorCenterWin()
{
    delete ui;
}

void SetColorCenterWin::initUi()
{
    ui->preSetColor->addItem("自定义");
    ui->preSetColor->addItem("默认");
    ui->preSetColor->addItem("绿色心情");
    ui->preSetColor->addItem("黑夜精灵");
    ui->preSetColor->addItem("黄金骑士");
    ui->preSetColor->addItem("红粉世家");
    ui->preSetColor->addItem("紫色浪漫");
    ui->preSetColor->addItem("蓝色梦幻");


    QPixmap pixmap(16,16);
    //1
    QStringList cl =QString(backColor).split(",");
    pixmap.fill(QColor(cl.value(0,"0").toInt(),cl.value(1,"112").toInt(),cl.value(2,"132").toInt()));
    ui->backColorCombo->addItem(QIcon(pixmap),backColor);
    ui->backColorCombo->setCurrentIndex(0);
    pixmap.fill(QColor(242,38,19));
    ui->backColorCombo->addItem(QIcon(pixmap),"242,38,19");
    pixmap.fill(QColor(150,40,27));
    ui->backColorCombo->addItem(QIcon(pixmap),"150,40,27");
    pixmap.fill(QColor(219,10,91));
    ui->backColorCombo->addItem(QIcon(pixmap),"219,10,91");
    pixmap.fill(QColor(246,36,89));
    ui->backColorCombo->addItem(QIcon(pixmap),"246,36,89");
    pixmap.fill(QColor(102,51,153));
    ui->backColorCombo->addItem(QIcon(pixmap),"102,51,153");
    pixmap.fill(QColor(103,65,114));
    ui->backColorCombo->addItem(QIcon(pixmap),"103,65,114");
    pixmap.fill(QColor(154,18,179));
    ui->backColorCombo->addItem(QIcon(pixmap),"154,18,179");
    pixmap.fill(QColor(191,85,236));
    ui->backColorCombo->addItem(QIcon(pixmap),"191,85,236");
    pixmap.fill(QColor(65,131,215));
    ui->backColorCombo->addItem(QIcon(pixmap),"65,131,215");
    pixmap.fill(QColor(89,171,227));
    ui->backColorCombo->addItem(QIcon(pixmap),"89,171,227");
    pixmap.fill(QColor(44,62,80));
    ui->backColorCombo->addItem(QIcon(pixmap),"44,62,80");
    pixmap.fill(QColor(25,181,254));
    ui->backColorCombo->addItem(QIcon(pixmap),"25,181,254");
    pixmap.fill(QColor(51,110,123));
    ui->backColorCombo->addItem(QIcon(pixmap),"51,110,123");
    pixmap.fill(QColor(30,139,195));
    ui->backColorCombo->addItem(QIcon(pixmap),"30,139,195");
    pixmap.fill(QColor(31,58,147));
    ui->backColorCombo->addItem(QIcon(pixmap),"31,58,147");
    pixmap.fill(QColor(37,116,169));
    ui->backColorCombo->addItem(QIcon(pixmap),"37,116,169");
    pixmap.fill(QColor(75,119,190));
    ui->backColorCombo->addItem(QIcon(pixmap),"75,119,190");
    pixmap.fill(QColor(78,205,196));
    ui->backColorCombo->addItem(QIcon(pixmap),"78,205,196");
    pixmap.fill(QColor(135,211,124));
    ui->backColorCombo->addItem(QIcon(pixmap),"135,211,124");
    pixmap.fill(QColor(38,166,91));
    ui->backColorCombo->addItem(QIcon(pixmap),"38,166,91");
    pixmap.fill(QColor(3,201,169));
    ui->backColorCombo->addItem(QIcon(pixmap),"3,201,169");
    pixmap.fill(QColor(46,204,113));
    ui->backColorCombo->addItem(QIcon(pixmap),"46,204,113");
    pixmap.fill(QColor(248,148,6));
    ui->backColorCombo->addItem(QIcon(pixmap),"248,148,6");
    pixmap.fill(QColor(248,148,6));
    ui->backColorCombo->addItem(QIcon(pixmap),"248,148,6");
    pixmap.fill(QColor(248,148,6));
    ui->backColorCombo->addItem(QIcon(pixmap),"248,148,6");
    pixmap.fill(QColor(245,171,53));
    ui->backColorCombo->addItem(QIcon(pixmap),"245,171,53");
    pixmap.fill(QColor(108,122,137));
    ui->backColorCombo->addItem(QIcon(pixmap),"108,122,137");
    pixmap.fill(QColor(149,165,166));
    ui->backColorCombo->addItem(QIcon(pixmap),"149,165,166");
    //2
    QStringList cl1 = QString(btnColor).split(",");
    pixmap.fill(QColor(cl1.value(0,"0").toInt(),cl1.value(1,"82").toInt(),cl1.value(2,"112").toInt()));
    ui->btnColorCombo->addItem(QIcon(pixmap),btnColor);
    ui->btnColorCombo->setCurrentIndex(0);
    pixmap.fill(QColor(242,38,19));
    ui->btnColorCombo->addItem(QIcon(pixmap),"242,38,19");
    pixmap.fill(QColor(150,40,27));
    ui->btnColorCombo->addItem(QIcon(pixmap),"150,40,27");
    pixmap.fill(QColor(219,10,91));
    ui->btnColorCombo->addItem(QIcon(pixmap),"219,10,91");
    pixmap.fill(QColor(246,36,89));
    ui->btnColorCombo->addItem(QIcon(pixmap),"246,36,89");
    pixmap.fill(QColor(102,51,153));
    ui->btnColorCombo->addItem(QIcon(pixmap),"102,51,153");
    pixmap.fill(QColor(103,65,114));
    ui->btnColorCombo->addItem(QIcon(pixmap),"103,65,114");
    pixmap.fill(QColor(154,18,179));
    ui->btnColorCombo->addItem(QIcon(pixmap),"154,18,179");
    pixmap.fill(QColor(191,85,236));
    ui->btnColorCombo->addItem(QIcon(pixmap),"191,85,236");
    pixmap.fill(QColor(65,131,215));
    ui->btnColorCombo->addItem(QIcon(pixmap),"65,131,215");
    pixmap.fill(QColor(89,171,227));
    ui->btnColorCombo->addItem(QIcon(pixmap),"89,171,227");
    pixmap.fill(QColor(44,62,80));
    ui->btnColorCombo->addItem(QIcon(pixmap),"44,62,80");
    pixmap.fill(QColor(25,181,254));
    ui->btnColorCombo->addItem(QIcon(pixmap),"25,181,254");
    pixmap.fill(QColor(51,110,123));
    ui->btnColorCombo->addItem(QIcon(pixmap),"51,110,123");
    pixmap.fill(QColor(30,139,195));
    ui->btnColorCombo->addItem(QIcon(pixmap),"30,139,195");
    pixmap.fill(QColor(31,58,147));
    ui->btnColorCombo->addItem(QIcon(pixmap),"31,58,147");
    pixmap.fill(QColor(37,116,169));
    ui->btnColorCombo->addItem(QIcon(pixmap),"37,116,169");
    pixmap.fill(QColor(75,119,190));
    ui->btnColorCombo->addItem(QIcon(pixmap),"75,119,190");
    pixmap.fill(QColor(78,205,196));
    ui->btnColorCombo->addItem(QIcon(pixmap),"78,205,196");
    pixmap.fill(QColor(135,211,124));
    ui->btnColorCombo->addItem(QIcon(pixmap),"135,211,124");
    pixmap.fill(QColor(38,166,91));
    ui->btnColorCombo->addItem(QIcon(pixmap),"38,166,91");
    pixmap.fill(QColor(3,201,169));
    ui->btnColorCombo->addItem(QIcon(pixmap),"3,201,169");
    pixmap.fill(QColor(46,204,113));
    ui->btnColorCombo->addItem(QIcon(pixmap),"46,204,113");
    pixmap.fill(QColor(248,148,6));
    ui->btnColorCombo->addItem(QIcon(pixmap),"248,148,6");
    pixmap.fill(QColor(248,148,6));
    ui->btnColorCombo->addItem(QIcon(pixmap),"248,148,6");
    pixmap.fill(QColor(248,148,6));
    ui->btnColorCombo->addItem(QIcon(pixmap),"248,148,6");
    pixmap.fill(QColor(245,171,53));
    ui->btnColorCombo->addItem(QIcon(pixmap),"245,171,53");
    pixmap.fill(QColor(108,122,137));
    ui->btnColorCombo->addItem(QIcon(pixmap),"108,122,137");
    pixmap.fill(QColor(149,165,166));
    ui->btnColorCombo->addItem(QIcon(pixmap),"149,165,166");


}

void SetColorCenterWin::setAppColor()
{
    QFile file(":/qss/skin/qss/template.qss");
    file.open(QFile::ReadOnly);
    QByteArray styleData = file.readAll();

    styleData.replace("backgroundColor",backColor);
    styleData.replace("btnColor",btnColor);
    qApp->setStyleSheet(styleData);
}

void SetColorCenterWin::on_backColorCombo_currentTextChanged(const QString &arg1)
{
    if(ui->backColorCombo->currentIndex() == 0)
    {
        return;
    }
    backColor = arg1.toLatin1();
    setAppColor();
}

void SetColorCenterWin::on_btnColorCombo_currentTextChanged(const QString &arg1)
{
    if(ui->btnColorCombo->currentIndex() == 0)
    {
        return;
    }

    btnColor = arg1.toLatin1();
    setAppColor();
}


void SetColorCenterWin::on_preSetColor_currentIndexChanged(int index)
{
    if(index == 0)
    {
        return;
    }else if(index == 1)
    {
        backColor = QByteArray("0,112,132");
        btnColor = QByteArray("0,82,112");
    }else if(index == 2)//绿色心情
    {
        backColor = QByteArray("46,204,113");
        btnColor = QByteArray("22,160,133");
    }else if(index == 3)//黑夜精灵
    {
        backColor = QByteArray("44,62,80");
        btnColor = QByteArray("52,73,94");
    }else if(index == 4)//黄金骑士
    {
        backColor = QByteArray("249,191,59");
        btnColor = QByteArray("249,105,14");
    }else if(index == 5)//红粉世家
    {
        backColor = QByteArray("246,36,89");
        btnColor = QByteArray("207,0,15");
    }else if(index == 6)//紫色浪漫
    {
        backColor = QByteArray("191,85,236");
        btnColor = QByteArray("154,18,179");
    }else if(index == 7)//蓝色梦幻
    {
        backColor = QByteArray("31,58,147");
        btnColor = QByteArray("75,119,190");
    }
    setAppColor();
}

void SetColorCenterWin::on_saveBtn_clicked()
{
    QSettings settings("MvGather","xusongjie");
    settings.setValue("app/backgroundColor",QString(backColor));
    settings.setValue("app/btnColor",QString(btnColor));
    emit closeS();
}

void SetColorCenterWin::on_backColorBtn_clicked()
{
    QStringList cl = QString(backColor).split(",");
    QColor rc = QColorDialog::getColor(QColor(cl.value(0,"0").toInt(),cl.value(1,"112").toInt(),cl.value(2,"132").toInt()),0,"选择程序背景色");
    if(!rc.isValid())
    {
        return;
    }
    QString b = QString(QString::number(rc.red())+","+QString::number(rc.green())+","+QString::number(rc.blue()));
    backColor = b.toLatin1();
    setAppColor();
}

void SetColorCenterWin::on_btnColorBtn_clicked()
{
    QStringList cl = QString(btnColor).split(",");
    QColor rc = QColorDialog::getColor(QColor(cl.value(0,"0").toInt(),cl.value(1,"82").toInt(),cl.value(2,"112").toInt()),0,"选择按钮颜色");
    if(!rc.isValid())
    {
        return;
    }
    QString b = QString(QString::number(rc.red())+","+QString::number(rc.green())+","+QString::number(rc.blue()));
    btnColor = b.toLatin1();
    setAppColor();
}
