#ifndef SETCOLORCENTERWIN_H
#define SETCOLORCENTERWIN_H

#include <QWidget>
#include <QFile>
#include <QSettings>
#include <QColorDialog>
namespace Ui {
class SetColorCenterWin;
}

class SetColorCenterWin : public QWidget
{
    Q_OBJECT

public:
    explicit SetColorCenterWin(QWidget *parent = 0);
    ~SetColorCenterWin();
    void initUi();
signals:
    void closeS();
private slots:
    void setAppColor();
    void on_backColorCombo_currentTextChanged(const QString &arg1);

    void on_btnColorCombo_currentTextChanged(const QString &arg1);

    void on_preSetColor_currentIndexChanged(int index);

    void on_saveBtn_clicked();

    void on_backColorBtn_clicked();

    void on_btnColorBtn_clicked();

private:
    Ui::SetColorCenterWin *ui;
    QByteArray backColor;
    QByteArray btnColor;
};

#endif // SETCOLORCENTERWIN_H
