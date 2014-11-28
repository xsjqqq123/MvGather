#include "prefersettingmainwin.h"

PreferSettingMainWin::PreferSettingMainWin(QWidget *parent) :
    FBaseDialog(parent)
{
    setWindowFlags(Qt::WindowStaysOnTopHint|Qt::FramelessWindowHint);
    this->normalSize = QSize(200,400);
    getTitleBar()->getTitleLabel()->setText("偏好设置(可拖放修改)");
    preferSetting = new PreferSetting(this);
    connect(preferSetting,SIGNAL(closeSig()),this,SLOT(close()));
    this->layout()->addWidget(preferSetting);
}
