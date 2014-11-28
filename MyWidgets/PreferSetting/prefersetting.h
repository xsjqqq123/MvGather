#ifndef PREFERSETTING_H
#define PREFERSETTING_H

#include <QWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QListWidget>

class PreferSetting : public QWidget//视频质量偏好设置
{
    Q_OBJECT
public:
    explicit PreferSetting(QWidget *parent = 0);

signals:
    void closeSig();
public slots:
    void saveQuality();
    void UpQuality();
    void DownQuality();
    void DeleteQuality();
    void AddQuality();
    void ResetQuality();
private:
    QGridLayout *mainLayout;
    QListWidget *qualityListWidget;
    QPushButton *qualityUpBtn;
    QPushButton *qualityDownBtn;
    QPushButton *qualityAddBtn;
    QPushButton *qualityDeleteBtn;
    QPushButton *qualityResetBtn;
    QPushButton *qualitySaveBtn;
    QString preferQualitysSetting;
};

#endif // PREFERSETTING_H
