#ifndef PREFERSOURCEWIDGET_H
#define PREFERSOURCEWIDGET_H

#include <QWidget>

namespace Ui {
class preferSourceWidget;
}

class preferSourceWidget : public QWidget
{
    Q_OBJECT

public:
    explicit preferSourceWidget(QWidget *parent = 0);
    ~preferSourceWidget();
signals:
    void closeSig();
private slots:
    void on_saveBtn_clicked();

    void on_upBtn_clicked();

    void on_downBtn_clicked();

    void on_resetBtn_clicked();

private:
    Ui::preferSourceWidget *ui;
};

#endif // PREFERSOURCEWIDGET_H
