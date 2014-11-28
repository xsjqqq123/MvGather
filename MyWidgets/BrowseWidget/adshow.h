#ifndef ADSHOW_H
#define ADSHOW_H

#include <QLabel>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkReply>
#include <QEventLoop>
#include <QTimer>
#include <QGridLayout>
class AdShow : public QWidget
{
    Q_OBJECT
public:
    explicit AdShow(QWidget *parent = 0);
signals:

public slots:
    void getData();
    void showPicAd(QString picUrl,QString txt);
private:
    QGridLayout *mainGird;
    QLabel *imgLbl;
    QLabel *txtLbl;
    QNetworkAccessManager *manager;
    QString urlToGo;
    QString host;
};

#endif // ADSHOW_H
