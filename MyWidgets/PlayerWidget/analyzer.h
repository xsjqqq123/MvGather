#ifndef ANALYZER_H
#define ANALYZER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QEventLoop>
#include <QTimer>
class Analyzer : public QObject//视频真实地址解析类
{
    Q_OBJECT
public:
    explicit Analyzer(QObject *parent = 0);

signals:

public slots:
    QString funTvAnalyze(QString url);//风行网视频地址解析

};

#endif // ANALYZER_H
