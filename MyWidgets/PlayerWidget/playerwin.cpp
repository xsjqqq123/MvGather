#include "playerwin.h"
#include <QPainter>
#include <QDebug>
#include <QMouseEvent>
#include <QMenu>
#include <QAction>
#include <QKeyEvent>
#include <QFileDialog>
#include <QInputDialog>
#include <QDebug>
#include <QActionGroup>
#include <QFont>
#include "qmath.h"
PlayerWin::PlayerWin(QWidget *parent) :
    QWidget(parent)
{
    setMinimumWidth(200);
    //setCursor(QCursor(Qt::UpArrowCursor));
    isFullScreen = false;
    state = 0;
    isWideModel = false;

    mainGLayout = new QGridLayout(this);
    mainGLayout->setMargin(0);
    mainGLayout->setSpacing(0);

    player= new  AVPlayer(this);
    renderer =  new GLWidgetRenderer(this);
    player->setRenderer(renderer);
    player->setAudioOutput(QtAV::AudioOutputFactory::create(QtAV::AudioOutputId_OpenAL));
    subFilter = new SubtitleFilter(this);

    subFilter->setPlayer(player);
    subFilter->installTo(player);
//    QFont f("Helvetica",35);
//    subFilter->setFont(f);
    subFilter->setAutoLoad(false);
    subFilter->setEngines(QStringList()<<"FFmpeg");//FFmpeg or LibASS
    subFilter->setCodec("AutoDetect");
    subFilter->setEnabled(true);
    subFilter->setRect(QRectF(0,0,1,1));
    //player->setAudioOutput(QtAV::AudioOutputFactory::create(QtAV::AudioOutputId_PortAudio));
    QVariantHash opt;
    opt["probesize"] = 4096;
    player->setOptionsForFormat(opt);

    mainGLayout->addWidget(renderer,0,0,1,1);

    //QImage img("://images/icon.png");

    //renderer->setOutAspectRatioMode(VideoRenderer::RendererAspectRatio);
    //    QImage img(5,5,QImage::Format_ARGB32);
    //    img.fill(Qt::black);
    //    renderer->receive(VideoFrame(img));
    renderer->setVisible(false);

    playerMaskLbl = new QLabel;
    playerMaskLbl->setAlignment(Qt::AlignCenter);
    playerMaskLbl->setStyleSheet("background-color:rgba(0,0,0,255)");
    QPixmap pix(":/images/images/icon.png");
    pix=pix.scaled(150,150,Qt::KeepAspectRatio,Qt::SmoothTransformation);
    playerMaskLbl->setPixmap(pix);
    mainGLayout->addWidget(playerMaskLbl,1,0,1,1);


    connect(player, SIGNAL(mediaStatusChanged(QtAV::MediaStatus)),
            this, SLOT(handleMediaStatusChanged(QtAV::MediaStatus)));
    connect(player, SIGNAL(error(QtAV::AVError)), this, SLOT(handleError(QtAV::AVError)));
    updateProgressBarT = new QTimer;
    connect(updateProgressBarT, SIGNAL(timeout()), this, SLOT(setProgressbarPosition()));
    updateProgressBarT->start(500);
    manager = new QNetworkAccessManager(this);
    EndOfMediaMark=0;

}

void PlayerWin::mouseDoubleClickEvent(QMouseEvent *event)
{
    setFullScreenMode();
    //QTimer::singleShot(200,this,SLOT(repaint()));
}

void PlayerWin::mousePressEvent(QMouseEvent *event)
{
    if(event->button()==Qt::RightButton)
    {
        QMenu *menu= new QMenu;
        QAction *pauseAct = new QAction(tr(this->state==1?"暂停&P":"播放&P"), this);
        QAction *stopAct = new QAction(tr("停止&S"), this);
        QAction *wideAct = new QAction(tr(this->isWideModel==true?"退出精简模式":"精简模式"), this);
        QAction *openAct = new QAction(tr("打开本地文件&O"), this);
        QAction *openUrlAct = new QAction(tr("打开Url"), this);

        QAction *loadAct = new QAction(tr("视频片切换"), this);
        QMenu *loadMenu= new QMenu;
        QAction *loadPreAct = new QAction(tr("上一片"), this);
        QAction *loadCurAct = new QAction(tr("重载当前"), this);
        QAction *loadNextAct = new QAction(tr("下一片"), this);
        loadMenu->addAction(loadPreAct);
        //loadMenu->addAction(loadCurAct);
        loadMenu->addAction(loadNextAct);
        loadAct->setMenu(loadMenu);


        QAction *audioOutputAct = new QAction(tr("声音输出"), this);
        QMenu *audioOutputMenu= new QMenu;
        QAction *openAlOutputAct = new QAction(tr("openAl"), this);
        QAction *portAudioOutputAct = new QAction(tr("PortAudio"), this);
        openAlOutputAct->setCheckable(true);
        portAudioOutputAct->setCheckable(true);/*
        if(player->audio()->AVOutput() == QtAV::AudioOutputId_OpenAL)
        {
            openAlOutputAct->setChecked(true);
        }else
        {
            portAudioOutputAct->setChecked(true);
        }*/
        audioOutputMenu->addAction(openAlOutputAct);
        audioOutputMenu->addAction(portAudioOutputAct);
        audioOutputAct->setMenu(audioOutputMenu);

        QAction *localSubAct = new QAction(tr("打开本地字幕"), this);
        QAction *closeSubAct = new QAction(tr("关闭本地字幕"), this);

        QAction *subAct = new QAction(tr("外挂字幕"), this);
        subAct->setCheckable(true);
        subAct->setChecked((subFilter->isEnabled())?true:false);

        connect(pauseAct, SIGNAL(triggered()), this, SLOT(setPauseState()));
        connect(stopAct, SIGNAL(triggered()), this, SLOT(setStopState()));
        connect(wideAct, SIGNAL(triggered()), this, SLOT(setWideModel()));
        connect(openAct, SIGNAL(triggered()), this, SLOT(openFile()));
        connect(openUrlAct, SIGNAL(triggered()), this, SLOT(openUrl()));
        connect(loadPreAct, SIGNAL(triggered()), this, SLOT(loadPreSlice()));
        connect(loadCurAct, SIGNAL(triggered()), this, SLOT(loadCurSlice()));
        connect(loadNextAct, SIGNAL(triggered()), this, SLOT(loadNextSlice()));
        connect(localSubAct, SIGNAL(triggered()), this, SLOT(loadLocalSub()));
        connect(closeSubAct, SIGNAL(triggered()), this, SLOT(closeSub()));
        connect(subAct, SIGNAL(triggered(bool)), this, SLOT(showSubtitle(bool)));

        menu->addAction(pauseAct);
        menu->addAction(stopAct);
        menu->addAction(wideAct);
        menu->addAction(openAct);
        menu->addAction(openUrlAct);
        menu->addSeparator();
        menu->addAction(loadAct);
        if(player->isPlaying() && player->file().startsWith("/"))
        {
            menu->addAction(localSubAct);
            if(!subFilter->file().isEmpty())
            {
                menu->addAction(closeSubAct);
            }
        }
        //menu->addAction(subAct);

        menu->addSeparator();

        if(subDownloadUrlList.count()>0 && subFilter->isEnabled() && player->isPlaying())
        {
            QAction *subListAct = new QAction(tr("字幕选择"), this);
            QMenu *subListMenu= new QMenu;
            subListAct->setMenu(subListMenu);
            QActionGroup *group = new QActionGroup(0);
            connect(group,SIGNAL(triggered(QAction*)),this,SLOT(changeSub(QAction*)));
            int count = 0;
            foreach (QString var, subDownloadUrlList) {
                QAction *act = new QAction(QString("字幕 %0").arg(count),this);
                act->setData(var);
                subListMenu->addAction(act);
                group->addAction(act);
                count++;
            }
            menu->addAction(subListAct);

        }
        //menu->addAction(audioOutputAct);
        QAction *returnAct = menu->exec(QCursor::pos());
        if(returnAct == openAlOutputAct)
        {
            player->setAudioOutput(QtAV::AudioOutputFactory::create(QtAV::AudioOutputId_OpenAL));
        }else if(returnAct == portAudioOutputAct)
        {
            player->setAudioOutput(QtAV::AudioOutputFactory::create(QtAV::AudioOutputId_PortAudio));
        }

    }

}

void PlayerWin::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setBrush(Qt::black);
    QPen pen;
    pen.setWidth(0);
    painter.setPen(pen);
    painter.drawRect(this->rect());
}

void PlayerWin::resizeEvent(QResizeEvent *event)
{
    //qDebug()<<"resize"<<event->size()<<this->mapToGlobal(this->pos());
}

//void PlayerWin::keyReleaseEvent(QKeyEvent *e)
//{
//    if(e->key()==Qt::Key_Space)
//    {
//        if(this->state==1)
//        {
//            setState(2);
//        }else if(this->state==2)
//        {
//            setState(1);
//        }

//    }
//}

//void PlayerWin::finished(int i)
//{
//    emit playFinished();
//}

void PlayerWin::setState(int i)
{
    if(i == 0)//停止
    {
        player->stop();
        subFilter->setEnabled(false);
        emit progressBarDrawTxt(QString("停止"));
        if(!playerMaskLbl->isVisible())
        {
            renderer->setVisible(false);
            playerMaskLbl->setVisible(true);
        }
    }else if(i==1)
    {
        player->togglePause();
        emit progressBarDrawTxt(QString("播放"));
    }else
    {
        player->togglePause();
        emit progressBarDrawTxt(QString("暂停"));
    }
    state = i;
}

void PlayerWin::setProgressbarPosition()
{
    if(player->isPlaying())
    {

        qreal posPercent = float(player->position()/float(player->duration()));
        //qDebug()<<player->position()<<player->duration()<<posPercent;
        posPercent = posPercent - qFloor(posPercent);
        emit percentChange(posPercent);
        if(posPercent>0.9 && EndOfMediaMark==0)
        {
            emit callForPlayer2Ready();
            EndOfMediaMark=1;
        }
    }

    repaint();

}

void PlayerWin::seekPos(qreal pos)
{
    if(!this->state==0)
    {
        player->seek(pos);
        //mplayerProc->write(QString("seek %1 1\n").arg(pos*100).toLatin1());
    }
}

void PlayerWin::setPauseState()
{
    if(this->state==1)
    {
        this->setState(2);
    }else if(this->state==2)
    {
        this->setState(1);
    }
}

void PlayerWin::setStopState()
{
    this->setState(0);
}

void PlayerWin::openFile()
{
//    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
//                                                    "/home");
    QFileDialog dialog(this);
    dialog.setObjectName("mydialog");
    //dialog.setStyleSheet("#mydialog{background:none}");
    dialog.setDirectory(QDir::home());
    QString fileName;
    if (dialog.exec())
    {
        fileName = dialog.selectedFiles().first();
        player->stop();
        player->play(fileName);
        this->state=1;
        emit resetSig();
    }
//    if(fileName =="")
//    {
//    }else
//    {
//        player->stop();
//        player->play(fileName);
//        this->state=1;
//    }
}

void PlayerWin::openUrl()
{
//    bool ok;
//    QString text = QInputDialog::getText(this, tr("输入url地址"),
//                                         tr("Url:"), QLineEdit::Normal,
//                                         "http://", &ok);
//    if (ok && !text.isEmpty())
//    {
//        player->stop();
//        player->play(text);
//        this->state=1;
//    }

    MyInputDialogMainWin *urlInputD = new MyInputDialogMainWin(0);
    urlInputD->normalSize = QSize(200,60);
    urlInputD->urlInputInit("http://");
    connect(urlInputD->centerWidget,SIGNAL(acceptSignal(QString)),this,SLOT(newPlay(QString)));
    connect(urlInputD->centerWidget,SIGNAL(acceptSignal(QString)),urlInputD,SLOT(close()));
    urlInputD->exec();
    emit resetSig();

}

void PlayerWin::setWideModel()
{
    if(isWideModel)
    {
        emit getIntoWideModel(true);
        isWideModel=false;
    }else
    {
        emit getIntoWideModel(false);
        isWideModel=true;
    }
}

void PlayerWin::setFullScreenMode()
{
    if(isFullScreen)
    {
        emit hideToFullScreen(true);
        isFullScreen=false;
    }else
    {
        emit hideToFullScreen(false);
        isFullScreen=true;
    }
}

void PlayerWin::loadSlice(int slice)
{
    emit sendloadSlice(slice);
}

void PlayerWin::loadPreSlice()
{
    loadSlice(-1);
}

void PlayerWin::loadCurSlice()
{
    loadSlice(0);
}

void PlayerWin::loadNextSlice()
{
    loadSlice(1);
}

void PlayerWin::handleMediaStatusChanged(MediaStatus status)
{
    if(status == QtAV::EndOfMedia)
    {
        emit playFinished();
        EndOfMediaMark=0;
    }else if(status == QtAV::LoadingMedia)
    {
        playerMaskLbl->setVisible(false);
        renderer->setVisible(true);
        emit loadingMedia();
    }/*else if(status == QtAV::LoadedMedia)
    {
        emit progressBarDrawTxt(QString("LoadedMedia"));
    }else if(status == QtAV::StalledMedia)
    {
        emit progressBarDrawTxt(QString("StalledMedia"));
    }*/else if(status == QtAV::BufferingMedia)
    {
        emit progressBarDrawTxt(QString("正在缓冲……"));
    }/*else if(status == QtAV::BufferedMedia)
    {
        emit progressBarDrawTxt(QString("BufferedMedia"));
    }*/
}

void PlayerWin::handleError(AVError error)
{
    //qDebug()<<error;
    myTipWin.show();
    myTipWin.setBusyIndicatorShow(false);
    myTipWin.setText("播放出错");
    myTipWin.timeToHide(3);
    if(!playerMaskLbl->isVisible())
    {
        playerMaskLbl->setVisible(true);
        renderer->setVisible(false);
    }
    player->stop();
}

void PlayerWin::newPlay(QString file)
{
    //player->stop();
    player->play(file);
    this->state=1;
}

void PlayerWin::setTotalSlices(int i)
{
    this->totalSlices=i;
}

void PlayerWin::showSubtitle(bool b)
{
    if(!b)
    {
        subFilter->setEnabled(false);
        return;
    }
    subFilter->setEnabled(true);
    //subFilter->setEngines("");
    QDir dir;
    QDir dir2(dir.homePath()+"/视频");
    QDir dir3(dir.homePath()+"/Videos");
    QString srtPath;
    if(dir2.exists())
    {
        srtPath = dir.homePath()+"/视频/MvGather/shooter.srt";
    }else if(dir3.exists())
    {
        srtPath = dir.homePath()+"/Videos/MvGather/shooter.srt";
    }else
    {
        srtPath = dir.homePath()+"/MvGather/shooter.srt";
    }
    getLocalFileSubList(player->file());
    subFilter->setFile(srtPath);
}

void PlayerWin::getLocalFileSubList(QString filePath)
{
    myTipWin.show();
    myTipWin.setBusyIndicatorShow(true);
    myTipWin.setText("加载列表");
    subDownloadUrlList.clear();
    QString fileHash = computerLocalFileHash(filePath);
//qDebug()<<"getLocalFileSub"<<fileHash;
    QNetworkRequest request(QUrl("http://www.shooter.cn/api/subapi.php"));

    QByteArray postData;
    postData.append(QString("filehash=%0&pathinfo=%1&format=json").arg(QString(fileHash.toUtf8().toPercentEncoding())).arg(QString(filePath.toUtf8().toPercentEncoding())));

    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");

    QNetworkReply *reply = manager->post(request,postData);
    QEventLoop loop;
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
    QByteArray jsonSource = reply->readAll();

    QJsonParseError *error=new QJsonParseError;
    QJsonDocument jsonDoc=QJsonDocument::fromJson(jsonSource,error);
    if(error->error==QJsonParseError::NoError)
    {
        QJsonArray rootArray = jsonDoc.array();
        foreach (QJsonValue subFileValue, rootArray) {
            QJsonObject subFileObj = subFileValue.toObject();
            QJsonArray FilesArray = subFileObj["Files"].toArray();
            foreach (QJsonValue fileValue, FilesArray) {
                QJsonObject fileObj = fileValue.toObject();
                if(fileObj["Ext"] == "srt")
                {
                    subDownloadUrlList.append(fileObj["Link"].toString());
                    //qDebug()<<fileObj["Link"];
                }
            }
        }
    }
    if(subDownloadUrlList.count()>0)
    {
        myTipWin.setText("下载字幕");
        getSubFileFromShooter(subDownloadUrlList.value(0,""));
    }else
    {
        myTipWin.setText("无匹配字幕");
    }
    myTipWin.timeToHide(1);

}

void PlayerWin::getSubFileFromShooter(QString Url)
{
    //qDebug()<<Url;
    QNetworkReply *reply = manager->get(QNetworkRequest(QUrl(Url.replace("https","http"))));
    QEventLoop loop;
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
    QByteArray srtFileData = reply->readAll();//

    QTextCodec *codecDefault = QTextCodec::codecForName("GB2312");
    QTextCodec *codec = QTextCodec::codecForUtfText(srtFileData,codecDefault);
    QString codecText= codec->toUnicode(srtFileData);

    QDir dir;
    QDir dir2(dir.homePath()+"/视频");
    QDir dir3(dir.homePath()+"/Videos");
    QString srtPath;
    if(dir2.exists())
    {
        srtPath = dir.homePath()+"/视频/MvGather/shooter.srt";
    }else if(dir3.exists())
    {
        srtPath = dir.homePath()+"/Videos/MvGather/shooter.srt";
    }else
    {
        srtPath = dir.homePath()+"/MvGather/shooter.srt";
    }
    QFile file(srtPath);
    file.open(QFile::WriteOnly|QFile::Truncate);
    QTextStream out(&file);
    QTextCodec *codecUtf8 = QTextCodec::codecForName("UTF-8");
    out.setCodec(codecUtf8);
    QByteArray a;
    a.append(codecText);
    out<<a;
}

QString PlayerWin::computerLocalFileHash(QString filePath)
{
    QStringList fileHashList;
    QFile file(filePath);
    file.open(QFile::ReadOnly);

    QFileInfo fileInfo(file);
    qint64 ftotallen = fileInfo.size();

    qint64 offset[4];
    offset[3] = ftotallen - 8192;
    offset[2] = ftotallen / 3;
    offset[1] = ftotallen / 3 * 2;
    offset[0] = 4096;

    file.seek(offset[0]);
    QByteArray data0 = file.read(4096);
    QByteArray md5_0 = QCryptographicHash::hash(data0,QCryptographicHash::Md5).toHex();
    fileHashList.append(md5_0);

    file.seek(offset[1]);
    QByteArray data1 = file.read(4096);
    QByteArray md5_1 = QCryptographicHash::hash(data1,QCryptographicHash::Md5).toHex();
    fileHashList.append(md5_1);

    file.seek(offset[2]);
    QByteArray data2 = file.read(4096);
    QByteArray md5_2 = QCryptographicHash::hash(data2,QCryptographicHash::Md5).toHex();
    fileHashList.append(md5_2);

    file.seek(offset[3]);
    QByteArray data3 = file.read(4096);
    QByteArray md5_3 = QCryptographicHash::hash(data3,QCryptographicHash::Md5).toHex();
    fileHashList.append(md5_3);

    return fileHashList.join(";");
}

void PlayerWin::changeSub(QAction *act)
{
    getSubFileFromShooter(act->data().toString());

    QDir dir;
    QDir dir2(dir.homePath()+"/视频");
    QDir dir3(dir.homePath()+"/Videos");
    QString srtPath;
    if(dir2.exists())
    {
        srtPath = dir.homePath()+"/视频/MvGather/shooter.srt";
    }else if(dir3.exists())
    {
        srtPath = dir.homePath()+"/Videos/MvGather/shooter.srt";
    }else
    {
        srtPath = dir.homePath()+"/MvGather/shooter.srt";
    }

    subFilter->setFile(srtPath);
}

void PlayerWin::loadLocalSub()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open subtittle"), QDir::homePath());
    if(!fileName.isEmpty())
    {
        subFilter->setFile(fileName);
    }
}

void PlayerWin::closeSub()
{
    subFilter->setFile("");
}

