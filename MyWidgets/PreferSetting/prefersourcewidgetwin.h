#ifndef PREFERSOURCEWIDGETWIN_H
#define PREFERSOURCEWIDGETWIN_H
#include "QFramer/fbasedialog.h"
#include "prefersourcewidget.h"
class PreferSourceWidgetWin : public FBaseDialog
{
    Q_OBJECT
public:
    explicit PreferSourceWidgetWin(QWidget *parent = 0);

signals:

public slots:
private:
    preferSourceWidget *prefer;

};

#endif // PREFERSOURCEWIDGETWIN_H
