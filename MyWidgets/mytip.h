#ifndef MYTIP_H
#define MYTIP_H

#include <QWidget>
#include <QTimer>
class MyTip : public QWidget
{
    Q_OBJECT
public:
    explicit MyTip(QWidget *parent = 0);
protected:
    void paintEvent(QPaintEvent *e);
    void mouseReleaseEvent(QMouseEvent *);
signals:

public slots:
    void setText(QString text);
    void timeToClose(int second);
    void timeToHide(int second);
    void busyAnglePlus();
    void setBusyIndicatorShow(bool on);
private:
    bool isBusyIndicatorShow;
    QString text;
    qreal busyAngle;
    QTimer *busyAnglePlusTimer;
};

#endif // MYTIP_H
