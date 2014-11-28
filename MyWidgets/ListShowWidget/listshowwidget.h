#ifndef LISTSHOWWIDGET_H
#define LISTSHOWWIDGET_H

#include <QWidget>
#include "listshowwidgetitem.h"
#include <QScrollArea>
#include <QGridLayout>
#include <QToolButton>
#include <QTimer>
class ListShowWidget : public QScrollArea
{
    Q_OBJECT
public:
    explicit ListShowWidget(QWidget *parent = 0);
    int page;
signals:
    void pageChanged();
    void clicked(QString,QString);
public slots:
    void loadDataToList(QStringList args);
    void pagePlus();
    void pageMinus();
    void showPage();
    void sendclicked(QString tvUrl,QString title);
    void loadSeeImg();
private:
    QGridLayout *mainLayout;
    QGridLayout *areaWidgetLayout;//areaWidget中组织item的layout
    ListShowWidgetItem *Item;
    QWidget *areaWidget;
    QToolButton *nextBtn;//下一页
    QToolButton *preBtn;//上一页
    QTimer *checkSeeImgTimer;
};

#endif // LISTSHOWWIDGET_H
