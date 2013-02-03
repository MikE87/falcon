#-------------------------------------------------
#
# Project created by QtCreator 2010-12-27T18:39:48
#
#-------------------------------------------------

QT       += core\
             gui\
          webkit\
         network

TARGET = Falcon
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    addressbar.cpp \
    browserwindow.cpp \
    historyview.cpp \
    settingsview.cpp \
    bookmarksview.cpp

HEADERS  += mainwindow.h \
    addressbar.h \
    browserwindow.h \
    historyview.h \
    settingsview.h \
    bookmarksview.h \
    keycodes.h

CONFIG +=
MOBILITY +=

symbian:ICON = icon.svg

symbian {
    TARGET.UID3 = 0xe8806138
    TARGET.CAPABILITY += NetworkServices
    TARGET.EPOCSTACKSIZE = 0x14000
    TARGET.EPOCHEAPSIZE = 0x020000 0x2000000
}

vendorinfo = \
     "%{\"MikE\"}" \
     ":\"MikE\""

 my_deployment.pkg_prerules = vendorinfo
 DEPLOYMENT += my_deployment
