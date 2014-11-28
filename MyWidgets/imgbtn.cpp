#include "imgbtn.h"

ImgBtn::ImgBtn(QWidget *parent, QString picPath) :
    QLabel(parent)
{
    setScaledContents(true);
    setPixmap(QPixmap(picPath));
    setToolTip("删除");
}

void ImgBtn::enterEvent(QEvent *event)
{
    setStyleSheet("background-color:rgba(255,255,255,20);border-radius:5px;");
}

void ImgBtn::leaveEvent(QEvent *event)
{
    setStyleSheet("background-color:rgba(255,255,255,0);");
}

void ImgBtn::mouseReleaseEvent(QMouseEvent *e)
{
    emit clicked();
}
