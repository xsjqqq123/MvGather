#include "mytip.h"
#include <QPainter>
#include <QTransform>
#include <QDebug>
#include <QDesktopWidget>
#include <QApplication>
MyTip::MyTip(QWidget *parent) :
    QWidget(parent)
{
    setWindowFlags(Qt::FramelessWindowHint|Qt::Tool | Qt::WindowStaysOnTopHint | Qt::X11BypassWindowManagerHint);
    setAttribute(Qt::WA_TranslucentBackground, true);
    resize(200,60);
    QDesktopWidget* desktop = QApplication::desktop();
    move(desktop->width()-this->width()-10, this->height()/2);
    busyAngle = 1.0;
    busyAnglePlusTimer = new QTimer(this);
    connect(busyAnglePlusTimer,SIGNAL(timeout()),SLOT(busyAnglePlus()));
    busyAnglePlusTimer->start(100);
    text = "busy";
    isBusyIndicatorShow = false;
}

void MyTip::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.save();
    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(QColor(10,10,10,200)));
    //painter.drawRect(this->rect());
    painter.drawRoundedRect(this->rect(),5,5);

    painter.setPen(Qt::white);
    painter.scale(1.2,1.2);
    painter.drawText(30,this->height()/2,text);
    painter.restore();
    if(isBusyIndicatorShow)
    {
        QPixmap pixBusy(":/images/images/tipBusy.png");
        painter.translate(this->width()-25,this->height()/2);
        painter.rotate(busyAngle);
        painter.scale(0.6,0.6);
        painter.drawPixmap(-20,-20,pixBusy);
    }
}

void MyTip::mouseReleaseEvent(QMouseEvent *)
{
    this->hide();
}

void MyTip::setText(QString text)
{
    this->text = text;
}

void MyTip::timeToClose(int second)
{
    QTimer::singleShot(second*1000,this,SLOT(close()));
}

void MyTip::timeToHide(int second)
{
    QTimer::singleShot(second*1000,this,SLOT(hide()));
}

void MyTip::busyAnglePlus()
{
    if(busyAngle >= 361.0)
    {
        busyAngle = 1.0;
    }
    busyAngle += 60.0;
    repaint();
}

void MyTip::setBusyIndicatorShow(bool on)
{
    isBusyIndicatorShow = on;
}
