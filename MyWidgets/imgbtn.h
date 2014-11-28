#ifndef IMGBTN_H
#define IMGBTN_H

#include <QLabel>

class ImgBtn : public QLabel//
{
    Q_OBJECT
public:
    explicit ImgBtn(QWidget *parent = 0,QString picPath="");
protected:
    virtual void enterEvent(QEvent * event);
    virtual void leaveEvent(QEvent * event);
    virtual void mouseReleaseEvent(QMouseEvent * e);
signals:
    void clicked();
public slots:

};

#endif // IMGBTN_H
