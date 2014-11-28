#ifndef SETCOLORWIDGET_H
#define SETCOLORWIDGET_H
#include "QFramer/fbasedialog.h"
#include "setcolorcenterwin.h"
class SetColorWidget : public FBaseDialog//皮肤颜色设置
{
    Q_OBJECT
public:
    explicit SetColorWidget(QWidget *parent = 0);

signals:

public slots:
private:
    SetColorCenterWin *setColorCenterWin;

};

#endif // SETCOLORWIDGET_H
