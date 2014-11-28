#ifndef PLAYERLISTAREA_H
#define PLAYERLISTAREA_H

#include <QScrollArea>
#include <QGridLayout>
#include "playerlistwidgetitem.h"
#include "../mytip.h"
#include <QContextMenuEvent>
class PlayerListArea : public QScrollArea//视频播放列表
{
    Q_OBJECT
public:
    explicit PlayerListArea(QWidget *parent = 0);
protected:
    void contextMenuEvent(QContextMenuEvent *event);
signals:
    void play(QString tvId,QString tvNo);
    void syncMv();
public slots:
    void updateList();
    void sendPlay(QString tvId,QString tvNo);
    void openUrl();
    void sendSyncMv();
private:
    QWidget *mainWidget;
    QGridLayout *mainWidgetGLayout;
    PlayerListWidgetItem *listWidgetItem;
    MyTip myTipWin;
};

#endif // PLAYERLISTAREA_H
