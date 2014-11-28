#ifndef VCODEDIALOG_H
#define VCODEDIALOG_H

#include <QDialog>

namespace Ui {
class VcodeDialog;
}

class VcodeDialog : public QDialog
{
    Q_OBJECT
public:
    explicit VcodeDialog(QWidget *parent = 0);
    ~VcodeDialog();
    void setPixmap(QPixmap pix);
    QString getText();
private:
    Ui::VcodeDialog *ui;
};

#endif // VCODEDIALOG_H
