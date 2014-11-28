#ifndef TVLISTWIDGET_H
#define TVLISTWIDGET_H

#include <QWidget>
#include <QTreeWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QEventLoop>
#include <QGridLayout>
#include <QActionGroup>
#include <QDebug>
#include <QFile>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QMenu>
#include <QAction>
#include <QTimer>

class TvListWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TvListWidget(QWidget *parent = 0);
signals:
    void play(QString url,QString quality);
public slots:
    void loadTvSources();
    void itemDoubleClicked(QTreeWidgetItem *item,int i);
    void customContextMenuRequested(QPoint p);
    void play(QAction *act);
private:
    QGridLayout *mainLayout;
    QTreeWidget *treeWidget;
    QActionGroup *actGoup;
};

#endif // TVLISTWIDGET_H
