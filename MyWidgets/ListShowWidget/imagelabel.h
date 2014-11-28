#ifndef IMAGELABEL_H
#define IMAGELABEL_H

#include <QLabel>
#include <QMouseEvent>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkReply>
#include "loadimgthread.h"
class ImageLabel : public QLabel
{
    Q_OBJECT
public:
    explicit ImageLabel(QObject *parent = 0,QString url="",int timeToDelay=0);//time表示推迟创建线程加载图片的时间
    bool isImgLoaded;
signals:
    void clicked();
protected:
    virtual void mouseReleaseEvent(QMouseEvent * e);
public slots:
    void getImage();
    void readImage(QByteArray data);
private:
    QString url;
    QList<LoadImgThread *> imgThreadList;

};

#endif // IMAGELABEL_H
