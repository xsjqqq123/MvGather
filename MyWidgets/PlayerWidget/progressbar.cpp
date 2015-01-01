#include "progressbar.h"
#include <QPainter>
#include <QPoint>
#include <QPixmap>
#include <QDebug>
#include "qmath.h"
#include <QPropertyAnimation>
ProgressBar::ProgressBar(QWidget *parent) :
    QWidget(parent)
{
    setAttribute(Qt::WA_TranslucentBackground, true);
    setMaximumHeight(4);
    posPercent=0;
    timeText ="Waited...";
    isMouseIn =false;
    progressBarHeight = 1;
    indicateLinePoint = QPoint(0,0);
    timer = new QTimer;
    connect(timer,SIGNAL(timeout()),this,SLOT(setSmallHeight()));
    timer->setInterval(2000);
    setMouseTracking(true);
}

ProgressBar::~ProgressBar()
{

}

void ProgressBar::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    QPen pen;
    painter.setRenderHint(QPainter::Antialiasing, true);
    pen.setWidth(0);
    painter.setBrush(Qt::black);
    painter.setPen(pen);
    painter.drawRect(this->rect());

    pen.setWidth(4);
    pen.setColor(Qt::darkGray);
    painter.setPen(pen);
    painter.drawLine(QPoint(5,6),QPoint(this->width()-5,6));

    pen.setColor(Qt::darkCyan);
    painter.setPen(pen);
    painter.drawLine(QPoint(5,6),QPoint((this->width()-10)*posPercent+5,6));

    pen.setColor(Qt::blue);
    pen.setWidth(1);
    painter.setPen(pen);
    painter.drawLine(indicateLinePoint.x(),3,indicateLinePoint.x(),this->height());

    pen.setColor(Qt::white);
    painter.setPen(pen);
    painter.drawText(5,30,this->timeText);
}

void ProgressBar::mouseReleaseEvent(QMouseEvent *)
{
    QPoint p =this->mapFromGlobal(QCursor::pos());
    if(p.x()>5 && p.x()<(this->width()-5))
    {
        qreal a = p.x()-5;
        qreal b = this->width()-10;
        posPercent=(a/b);
        emit posChange(posPercent);
    }
    repaint();
}

void ProgressBar::mouseMoveEvent(QMouseEvent *e)
{
    indicateLinePoint = e->pos();
    repaint();
}

void ProgressBar::leaveEvent(QEvent * event)
{
    isMouseIn = false;
    setSmallHeight();
}

void ProgressBar::enterEvent(QEvent *event)
{
    isMouseIn = true;
    setBigHeight();
}

void ProgressBar::setPercentPos(qreal posPercent)
{
    if(posPercent<1 && posPercent>0)
    {
        this->posPercent = posPercent;
    }else
    {
        this->posPercent = posPercent - qFloor(posPercent);
    }

    repaint();
}
void ProgressBar::drawTimeText(QString timeText)
{
    this->timeText = timeText;
    setMaximumHeight(40);
    repaint();
    if(timer->isActive())
    {
        timer->stop();
        timer->start();
    }else
    {
        timer->start();
    }
    if(timeText.contains("暂停"))
    {
        timer->stop();
    }
}

void ProgressBar::setSmallHeight()
{
    if(!isMouseIn)
    {
        progressBarHeight=1;
        setMaximumHeight(4);
    }

}

void ProgressBar::setBigHeight()
{
    progressBarHeight=10;
    setMaximumHeight(40);

}
