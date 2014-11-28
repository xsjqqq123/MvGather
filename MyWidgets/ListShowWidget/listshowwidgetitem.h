#ifndef LISTSHOWWIDGETITEM_H
#define LISTSHOWWIDGETITEM_H

#include <QWidget>
#include <QGridLayout>
#include "imagelabel.h"
#include <QIcon>
#include <QLabel>
class ListShowWidgetItem : public QWidget
{
    Q_OBJECT
public:
    explicit ListShowWidgetItem(QWidget *parent = 0,QString tvUrl="",QString imgUrl="",QString title="",QString descripe="",int timeToDelay=0);
    ImageLabel *ImgLbl;
protected:
    //void paintEvent(QPaintEvent *);
signals:
    void clicked(QString tvUrl,QString title);
public slots:
    void SendClicked();
    void loadImg();
    void backgroundColorChange();
private:
    QString tvUrl;
    QString imgUrl;
    QString title;
    QString descripe;
    QGridLayout *mainGLayout;

    QLabel *titleLbl;
    QLabel *descripeLbl;
};

#endif // LISTSHOWWIDGETITEM_H
