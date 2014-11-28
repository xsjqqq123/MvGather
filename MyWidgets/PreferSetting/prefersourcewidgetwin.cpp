#include "prefersourcewidgetwin.h"

PreferSourceWidgetWin::PreferSourceWidgetWin(QWidget *parent)
{
    setWindowFlags(Qt::WindowStaysOnTopHint|Qt::FramelessWindowHint);
    this->normalSize = QSize(200,300);
    getTitleBar()->getTitleLabel()->setText("视频源偏好设置");
    prefer = new preferSourceWidget(this);
    connect(prefer,SIGNAL(closeSig()),this,SLOT(close()));
    this->layout()->addWidget(prefer);
}
