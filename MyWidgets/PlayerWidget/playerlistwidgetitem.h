#ifndef PLAYERLISTWIDGETITEM_H
#define PLAYERLISTWIDGETITEM_H

#include <QWidget>
#include "../imgbtn.h"
#include <QGridLayout>
#include <QToolButton>
#include <QButtonGroup>
#include <QHBoxLayout>
class PlayerListWidgetItem : public QWidget
{
    Q_OBJECT
public:
    explicit PlayerListWidgetItem(QWidget *parent = 0,QString tvId="0",QString tvName="none");

signals:
    void deleteItem(QString tvId);
    void sendPlay(QString tvId,QString tvNo);
    void updatePlayList();
public slots:
    void addSerial(QString tvNo,QString historyNo);
    void play(int btNo);
    void updateHistoryNo(int historyNo);
    int getRow();
    void sendUpdatePlayList();
private:
    QGridLayout *mainGLayout;
    QHBoxLayout *hLayout;
    QString tvId;
    ImgBtn *deleteBtn;
    QToolButton *serialBtn;
    QButtonGroup *serialBtnGroup;
    int row;
    int column;
};

#endif // PLAYERLISTWIDGETITEM_H
