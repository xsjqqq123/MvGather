#ifndef MYINPUTDIALOG_H
#define MYINPUTDIALOG_H
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QToolButton>
#include <QSpacerItem>
#include <QGridLayout>
class MyInputDialog : public QWidget
{
    Q_OBJECT
public:
    explicit MyInputDialog(QWidget *parent = 0);

signals:
    void acceptSignal(QString inputText);
public slots:
    void setPixMap(QPixmap pix);
    void setLblText(QString txt);
    void setLineEditText(QString txt);
    QString getText();
    void emitAccept();
private:
    QGridLayout *mainLayout;
    QLabel *txtOrImgLbl;
    QLineEdit *inputL;
    QToolButton *okBtn;
    QSpacerItem *spaceItem;

};

#endif // MYINPUTDIALOG_H
