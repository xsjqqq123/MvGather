#ifndef PROGRESSBAR_H
#define PROGRESSBAR_H

#include <QWidget>
#include <QTimer>
class ProgressBar : public QWidget
{
    Q_OBJECT
public:
    explicit ProgressBar(QWidget *parent = 0);
    ~ProgressBar();
protected:
    void paintEvent(QPaintEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void leaveEvent(QEvent * event);
    void enterEvent(QEvent * event);

signals:
    void posChange(qreal posPercent);
public slots:
    void setPercentPos(qreal posPercent);
    void drawTimeText(QString timeText);
    void setSmallHeight();
    void setBigHeight();
private:
    qreal posPercent;
    QString timeText;
    QTimer *timer;
    int progressBarHeight;
    bool isMouseIn;
};

#endif // PROGRESSBAR_H
