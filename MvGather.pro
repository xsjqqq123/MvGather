#-------------------------------------------------
#
# Project created by QtCreator 2014-11-09T22:59:28
#
#-------------------------------------------------

QT       += core gui network sql av

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MvGather
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    centerwindow.cpp \
    MyWidgets/PlayerWidget/analyzer.cpp \
    MyWidgets/PlayerWidget/playerlistarea.cpp \
    MyWidgets/PlayerWidget/playerlistwidgetitem.cpp \
    MyWidgets/PlayerWidget/playerwidget.cpp \
    MyWidgets/PlayerWidget/playerwin.cpp \
    MyWidgets/PlayerWidget/progressbar.cpp \
    MyWidgets/PlayerWidget/tvlistwidget.cpp \
    MyWidgets/imgbtn.cpp \
    MyWidgets/mytip.cpp \
    MyWidgets/BrowseWidget/adshow.cpp \
    MyWidgets/BrowseWidget/browsewidget.cpp \
    MyWidgets/ListShowWidget/imagelabel.cpp \
    MyWidgets/ListShowWidget/listshowwidget.cpp \
    MyWidgets/ListShowWidget/listshowwidgetitem.cpp \
    MyWidgets/ListShowWidget/loadimgthread.cpp \
    MyWidgets/RecommendWidget/recommendwidget.cpp \
    MyWidgets/MagnetWidget/loginform.cpp \
    MyWidgets/MagnetWidget/magnetwidget.cpp \
    MyWidgets/MagnetWidget/vcodedialog.cpp \
    MyWidgets/PreferSetting/prefersetting.cpp \
    MyWidgets/PreferSetting/prefersettingmainwin.cpp \
    MyWidgets/MagnetWidget/loginformwin.cpp \
    MyWidgets/MyDialog/myinputdialog.cpp \
    MyWidgets/MyDialog/myinputdialogmainwin.cpp \
    MyWidgets/SetColorWidget/setcolorwidget.cpp \
    MyWidgets/SetColorWidget/setcolorcenterwin.cpp \
    MyWidgets/PreferSetting/prefersourcewidget.cpp \
    MyWidgets/PreferSetting/prefersourcewidgetwin.cpp \
    MyWidgets/ThreadDownload/downloadwidget.cpp

HEADERS  += mainwindow.h \
    centerwindow.h \
    MyWidgets/PlayerWidget/analyzer.h \
    MyWidgets/PlayerWidget/playerlistarea.h \
    MyWidgets/PlayerWidget/playerlistwidgetitem.h \
    MyWidgets/PlayerWidget/playerwidget.h \
    MyWidgets/PlayerWidget/playerwin.h \
    MyWidgets/PlayerWidget/progressbar.h \
    MyWidgets/PlayerWidget/tvlistwidget.h \
    MyWidgets/imgbtn.h \
    MyWidgets/mytip.h \
    MyWidgets/BrowseWidget/adshow.h \
    MyWidgets/BrowseWidget/browsewidget.h \
    MyWidgets/ListShowWidget/imagelabel.h \
    MyWidgets/ListShowWidget/listshowwidget.h \
    MyWidgets/ListShowWidget/listshowwidgetitem.h \
    MyWidgets/ListShowWidget/loadimgthread.h \
    MyWidgets/RecommendWidget/recommendwidget.h \
    MyWidgets/MagnetWidget/loginform.h \
    MyWidgets/MagnetWidget/magnetwidget.h \
    MyWidgets/MagnetWidget/vcodedialog.h \
    MyWidgets/PreferSetting/prefersetting.h \
    MyWidgets/PreferSetting/prefersettingmainwin.h \
    MyWidgets/MagnetWidget/loginformwin.h \
    MyWidgets/MyDialog/myinputdialog.h \
    MyWidgets/MyDialog/myinputdialogmainwin.h \
    MyWidgets/SetColorWidget/setcolorwidget.h \
    MyWidgets/SetColorWidget/setcolorcenterwin.h \
    MyWidgets/PreferSetting/prefersourcewidget.h \
    MyWidgets/PreferSetting/prefersourcewidgetwin.h \
    MyWidgets/ThreadDownload/downloadwidget.h

include(./QFramer/QFramer.pri)

RESOURCES += \
    rs.qrc

FORMS += \
    MyWidgets/MagnetWidget/loginform.ui \
    MyWidgets/MagnetWidget/magnetwidget.ui \
    MyWidgets/MagnetWidget/vcodedialog.ui \
    MyWidgets/SetColorWidget/setcolorcenterwin.ui \
    MyWidgets/PreferSetting/prefersourcewidget.ui \
    MyWidgets/ThreadDownload/downloadwidget.ui
