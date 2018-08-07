QT += qml quick widgets network xml sql webview multimedia
CONFIG += c++14

TARGET = QT_Media_Server
TEMPLATE = app

CONFIG(release, debug|release):DEFINES += QT_NO_DEBUG_OUTPUT

INCLUDEPATH += $$(MYLIBRARY)/$$QT_VERSION/include/QmlApplication
LIBS += -L$$(MYLIBRARY)/$$QT_VERSION -l$$qtLibraryTarget(QmlApplication)

INCLUDEPATH += $$(MYLIBRARY)/$$QT_VERSION/include/Youtube
LIBS += -L$$(MYLIBRARY)/$$QT_VERSION -l$$qtLibraryTarget(youtube)

INCLUDEPATH += $$(MYLIBRARY)/$$QT_VERSION/include/UpnpLibrary
LIBS += -L$$(MYLIBRARY)/$$QT_VERSION -l$$qtLibraryTarget(UpnpLibrary)

INCLUDEPATH += $$(MYLIBRARY)/$$QT_VERSION/include/multimedia
LIBS += -L$$(MYLIBRARY)/$$QT_VERSION -l$$qtLibraryTarget(mediadevice)

INCLUDEPATH += /opt/local/include
LIBS += -L/opt/local/lib -lavcodec -lavformat -lavutil -lswscale -lswresample

DEFINES += USE_AVRESAMPLE
LIBS += -L/opt/local/lib -lavresample

HEADERS += \
    myapplication.h \
    applicationcontroller.h \
    debugitem.h \
    checknetworklinkitem.h \
    applicationworker.h \
    mediaimageprovider.h \
    albumimageprovider.h \
    formatimageprovider.h \
    mediaserver.h

SOURCES += \
    main.cpp \
    myapplication.cpp \
    applicationcontroller.cpp \
    debugitem.cpp \
    checknetworklinkitem.cpp \
    applicationworker.cpp \
    mediaimageprovider.cpp \
    albumimageprovider.cpp \
    formatimageprovider.cpp \
    mediaserver.cpp

RESOURCES += \
    ressources.qrc

mac {
    ICON = icon.icns
} else {
    RC_ICONS = icon.ico
}

DISTFILES += \
    app.iconset/icon_512x512.png \
    app.iconset/icon_512x512@2x.png \
    app.iconset/icon_128x128.png \
    app.iconset/icon_128x128@2x.png \
    app.iconset/icon_32x32.png \
    app.iconset/icon_32x32@2x.png \
    app.iconset/icon_16x16.png \
    app.iconset/icon_16x16@2x.png \
    app.iconset/icon_256x256.png \
    app.iconset/icon_256x256@2x.png \
    icon.icns \
    UPnP_AV_ConnectionManager_1.0.xml \
    UPnP_AV_ContentDirectory_1.0.xml \
    icon.ico
