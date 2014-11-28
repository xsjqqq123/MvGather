#ifndef PREFERSETTINGMAINWIN_H
#define PREFERSETTINGMAINWIN_H
#include "QFramer/fbasedialog.h"
#include "prefersetting.h"

class PreferSettingMainWin : public FBaseDialog//视频质量偏好设置
{
    Q_OBJECT
public:
    explicit PreferSettingMainWin(QWidget *parent = 0);

signals:

public slots:
private:
    PreferSetting *preferSetting;

};

#endif // PREFERSETTINGMAINWIN_H
