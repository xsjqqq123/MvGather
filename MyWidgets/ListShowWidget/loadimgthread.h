#ifndef LOADIMGTHREAD_H
#define LOADIMGTHREAD_H

#include <QThread>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
class LoadImgThread : public QThread
{
    Q_OBJECT
public:
    explicit LoadImgThread(QObject *parent = 0,QString url="");
protected:
    void run();
signals:
    void imageReady(QByteArray data);
private:
    QNetworkAccessManager *manager;
    QString url;
};

#endif // LOADIMGTHREAD_H
