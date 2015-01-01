#include "mainwindow.h"
#include "QFramer/fmainwindow.h"
#include <QDesktopWidget>
#include <QApplication>
#include <QBitmap>
MainWindow::MainWindow(QWidget *parent)
    : FMainWindow(parent)
{
    centerWindow = new CenterWindow;
    setCentralWidget(centerWindow);
    getTitleBar()->getTitleLabel()->setText("影视集结号3.2.0");
    getTitleBar()->getSkinButton()->setVisible(false);
    getTitleBar()->getFixButton()->setVisible(false);
    getTitleBar()->getLogoButton()->setIcon(QIcon(":/images/images/icon.png"));
    getQSystemTrayIcon()->setIcon(QIcon(":/images/images/icon.png"));
    //getStatusBar()->setMaximumHeight(20);

    connect(centerWindow,SIGNAL(getIntofullScreenSignal(bool)),this,SLOT(getIntofullScreenMode(bool)));
    connect(centerWindow,SIGNAL(getIntoWideSignal(bool)),this,SLOT(getIntoWideMode(bool)));

    connect(getTitleBar()->getSettingButton(),SIGNAL(clicked()),this,SLOT(popupSettingMenu()));
    connect(getTitleBar()->getLogoButton(),SIGNAL(clicked()),this,SLOT(toggleStayOnTop()));

    isNowStayOnTop = false;

    appColorChange();
}

void MainWindow::keyPressEvent(QKeyEvent *keyEvent)
{
    if (keyEvent->key() == Qt::Key_Space) {
        if(centerWindow->playerWidget->playerWin->state==1)
        {
            centerWindow->playerWidget->playerWin->setState(2);
        }else if(centerWindow->playerWidget->playerWin->state==2)
        {
            centerWindow->playerWidget->playerWin->setState(1);
        }
    }else if(keyEvent->key() == Qt::Key_Left)
    {
        centerWindow->playerWidget->playerWin->player->seekBackward();
    }else if(keyEvent->key() == Qt::Key_Right)
    {
        centerWindow->playerWidget->playerWin->player->seekForward();
    }else if(keyEvent->key() == Qt::Key_Escape)
    {
        centerWindow->playerWidget->playerWin->setFullScreenMode();
    }
}
/*

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("text/uri-list")) {
        event->acceptProposedAction();
    }
}

void MainWindow::dropEvent(QDropEvent *event)
{
    QList<QUrl> urls = event->mimeData()->urls();
    if (urls.isEmpty()) {
        return;
    }

    QString fileName = urls.first().toLocalFile();
    if (fileName.isEmpty()) {
        return;
    }
    centerWindow->play(fileName);
}*/

MainWindow::~MainWindow()
{

}

void MainWindow::getIntofullScreenMode(bool b)
{
    if(b)
    {
        showNormal();
    }else
    {
        showFullScreen();
    }
    getTitleBar()->setVisible(b);
    getStatusBar()->setVisible(b);
}

void MainWindow::getIntoWideMode(bool b)
{
    getStatusBar()->setVisible(b);
}

void MainWindow::popupSettingMenu()
{
    QMenu *menu= new QMenu;
    QAction *preferSetAct = new QAction(tr("视频质量优先级设置"), this);
    QAction *preferSourceAct = new QAction(tr("视频来源优先级设置"), this);
    QAction *preferColorAct = new QAction(tr("软件皮肤设置"), this);
    QAction *checkVAct = new QAction(tr("检查更新"), this);

    menu->addAction(preferSetAct);
    menu->addAction(preferSourceAct);
    menu->addAction(preferColorAct);
    menu->addAction(checkVAct);

    QAction *resultAct = menu->exec(QCursor::pos());
    if(resultAct == preferSetAct)
    {
        PreferSettingMainWin *s = new PreferSettingMainWin(0);
        s->show();

    }else if(resultAct == checkVAct)
    {
        centerWindow->checkVersion();
    }else if(resultAct == preferColorAct)
    {
        setColorWidget = new SetColorWidget;
        setColorWidget->show();
    }else if(resultAct == preferSourceAct)
    {
        PreferSourceWidgetWin *s = new PreferSourceWidgetWin(0);
        s->show();
    }
    getTitleBar()->clearChecked();
}

void MainWindow::toggleStayOnTop()
{
    QMenu *menu = new QMenu;
    QAction *onTopAct = new QAction(menu);
    onTopAct->setText("置顶");
    onTopAct->setCheckable(true);
    onTopAct->setChecked(isNowStayOnTop?true:false);
    menu->addAction(onTopAct);
    QAction *returnAct = menu->exec(QCursor::pos());
    if(returnAct == onTopAct)
    {

        if(isNowStayOnTop)
        {
            setWindowFlags(Qt::FramelessWindowHint);
            isNowStayOnTop = false;

        }else
        {
            setWindowFlags(Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint);
            isNowStayOnTop = true;
        }
    }
    this->show();
    getTitleBar()->clearChecked();
    return;
}
void MainWindow::appColorChange()
{
    QSettings settings("MvGather", "xusongjie");
    QByteArray backgroundColor = settings.value("app/backgroundColor", "0,112,132").toByteArray();
    QByteArray btnColor = settings.value("app/btnColor", "0, 82, 112").toByteArray();

    QFile file(":/qss/skin/qss/template.qss");
    file.open(QFile::ReadOnly);
    QByteArray styleData = file.readAll();
    styleData.replace("backgroundColor",backgroundColor);//background color change
    styleData.replace("btnColor",btnColor);//some button color change
    qApp->setStyleSheet(styleData);
}
