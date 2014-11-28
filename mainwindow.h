#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
#include <QMenu>
#include "centerwindow.h"
#include "QFramer/fmainwindow.h"
#include "MyWidgets/PreferSetting/prefersettingmainwin.h"
#include "MyWidgets/SetColorWidget/setcolorwidget.h"
#include "MyWidgets/PreferSetting/prefersourcewidgetwin.h"
class MainWindow : public FMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    CenterWindow* centerWindow;
protected:
    //void closeEvent(QCloseEvent *event);
    //void contextMenuEvent(QContextMenuEvent *event);
    void keyPressEvent(QKeyEvent *keyEvent);
//    void dragEnterEvent(QDragEnterEvent *event);
//    void dropEvent(QDropEvent *event);
public slots:
    void getIntofullScreenMode(bool b);
    void getIntoWideMode(bool b);
    void popupSettingMenu();
    void toggleStayOnTop();
    void appColorChange();
private:

    bool isNowStayOnTop;
    SetColorWidget *setColorWidget;
};

#endif // MAINWINDOW_H
