# -------------------------------------------------
# Project created by QtCreator 2009-07-22T12:27:58
# -------------------------------------------------
QT += webkit network
TARGET = surfer
TEMPLATE = app
DESTDIR = bin
MOC_DIR = build
OBJECTS_DIR = build
RCC_DIR = build
UI_DIR = build
SOURCES += src/main.cpp \
    src/mainwindow.cpp \
    src/webview.cpp \
    src/tabmanager.cpp \
    src/webtab.cpp \
    src/addressbar.cpp \
    src/historymanager.cpp \
    src/historyitem.cpp \
    src/historyview.cpp \
    src/historytab.cpp \
    src/bookmarksmanager.cpp \
    src/bookmarksitem.cpp \
    src/bookmarksview.cpp \
    src/bookmarkstab.cpp \
    src/settingsmanager.cpp \
    src/settingsdialog.cpp \
    src/webpage.cpp \
    src/networkaccessmanager.cpp \
    src/authenticationdialog.cpp \
    src/downloadmanager.cpp \
    src/downloaditem.cpp \
    src/downloadview.cpp \
    src/downloadtab.cpp \
    src/searchbar.cpp \
    src/zoomslider.cpp \
    src/application.cpp \
    src/downloaditemdelegate.cpp \
    src/bookmarksfoldermodel.cpp \
    src/bookmarksfolderview.cpp \
    src/bookmarksfolderdialog.cpp \
    src/restoreitem.cpp \
    src/bookmarksdelegate.cpp \
    src/backbutton.cpp \
    src/forwardbutton.cpp \
    src/reloadstopbutton.cpp \
    src/restorebutton.cpp \
    src/historybutton.cpp \
    src/downloadbutton.cpp \
    src/settingsbutton.cpp \
    src/bookmarksbutton.cpp \
    src/networkcookiejar.cpp \
    src/menu.cpp \
    src/global.cpp \
    src/itab.cpp \
    src/zoombar.cpp \
    src/historyinterface.cpp \
    src/bookmarksfiltermodel.cpp
HEADERS += src/mainwindow.h \
    src/webview.h \
    src/tabmanager.h \
    src/webtab.h \
    src/addressbar.h \
    src/historymanager.h \
    src/historyitem.h \
    src/historyview.h \
    src/historytab.h \
    src/bookmarksmanager.h \
    src/bookmarksitem.h \
    src/bookmarkstab.h \
    src/bookmarksview.h \
    src/settingsmanager.h \
    src/settingsdialog.h \
    src/webpage.h \
    src/networkaccessmanager.h \
    src/authenticationdialog.h \
    src/downloadmanager.h \
    src/downloaditem.h \
    src/downloadview.h \
    src/downloadtab.h \
    src/searchbar.h \
    src/zoomslider.h \
    src/application.h \
    src/downloaditemdelegate.h \
    src/bookmarksfoldermodel.h \
    src/bookmarksfolderview.h \
    src/bookmarksfolderdialog.h \
    src/restoreitem.h \
    src/bookmarksdelegate.h \
    src/backbutton.h \
    src/forwardbutton.h \
    src/reloadstopbutton.h \
    src/restorebutton.h \
    src/historybutton.h \
    src/downloadbutton.h \
    src/settingsbutton.h \
    src/bookmarksbutton.h \
    src/networkcookiejar.h \
    src/menu.h \
    src/global.h \
    src/itab.h \
    src/zoombar.h \
    src/historyinterface.h \
    src/bookmarksfiltermodel.h
FORMS += src/settingsdialog.ui
RESOURCES += surfer.qrc
TRANSLATIONS += translations/ru_RU.ts \
    translations/pt_BR.ts \
    translations/es_ES.ts
os2: RC_FILE = surfer_os2.rc