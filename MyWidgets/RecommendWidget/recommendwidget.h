#ifndef RECOMMENDWIDGET_H
#define RECOMMENDWIDGET_H

#include <QWidget>
#include "../ListShowWidget/listshowwidget.h"
#include <QGridLayout>
#include <QToolButton>
#include <QLineEdit>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkReply>
#include <QEventLoop>
#include <QTimer>
#include "../mytip.h"
class RecommendWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RecommendWidget(QWidget *parent = 0);
    bool isFirstLoad;
signals:

    void addDownloadTaskSignal(QString url);
public slots:
    void loadDataFromServer(int page);
    void loadData();
    void reLoadData();

    void addDownloadTask(QString url,QString title);
    void search();
    void counter();//服务器计数
private:
    QNetworkAccessManager *manager;
    QGridLayout *mainGLayout;
    ListShowWidget *listShowWidget;
    QToolButton *dyRecommendBtn;
    QToolButton *searchBtn;
    QLineEdit *searchLineEdit;
    QString hostName;
    MyTip myTipWin;
};

#endif // RECOMMENDWIDGET_H
