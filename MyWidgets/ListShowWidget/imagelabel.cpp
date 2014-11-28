#include "imagelabel.h"
#include <QPixmap>
#include <QDebug>
#include <QEventLoop>
#include <QUrl>
#include <QTimer>
#include <QGraphicsDropShadowEffect>
ImageLabel::ImageLabel(QObject *parent, QString url, int timeToDelay)
{
    setAttribute(Qt::WA_TranslucentBackground, true);
    //setScaledContents(true);
//    setMaximumSize(260,290);
//    setMinimumSize(240,280);
    setMaximumSize(115,160);
    setMinimumSize(115,160);
    //setStyleSheet("border-width: 3px;border-color: rgba(255,170,0,150);");

    this->url=url;
    QPixmap p;
    p.load(":/images/images/load.jpg");
    p = p.scaled(this->width(),this->height(),Qt::KeepAspectRatio);
    setPixmap(p);
    //QTimer::singleShot(timeToDelay,this,SLOT(getImage()));
    isImgLoaded =false;

    QGraphicsDropShadowEffect *shadow_effect = new QGraphicsDropShadowEffect(this);
    shadow_effect->setOffset(3, 3);
    shadow_effect->setColor(Qt::black);
    shadow_effect->setBlurRadius(5);
    setGraphicsEffect(shadow_effect);
}

void ImageLabel::mouseReleaseEvent(QMouseEvent *e)
{
    Q_UNUSED(e)
    emit clicked();
}
void ImageLabel::getImage()
{
    foreach (LoadImgThread *t, imgThreadList) {
        t->terminate();
    }
    imgThreadList.clear();
    LoadImgThread *t1 = new LoadImgThread(0,this->url);
    imgThreadList.append(t1);
    connect(t1,SIGNAL(imageReady(QByteArray)),this,SLOT(readImage(QByteArray)));
    connect(t1,SIGNAL(finished()),t1,SLOT(deleteLater()));
    t1->start();
}

void ImageLabel::readImage(QByteArray data)
{
    QPixmap pix;
    if(pix.loadFromData(data))
    {
        pix = pix.scaled(this->width(),this->height(),Qt::KeepAspectRatio,Qt::SmoothTransformation);
        this->setPixmap(pix);

    }

}

