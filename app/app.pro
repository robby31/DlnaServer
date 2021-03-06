QT += qml quick widgets network xml sql webview multimedia
CONFIG += c++14
CONFIG += sdk_no_version_check

TARGET = QT_Media_Server
TEMPLATE = app

CONFIG(release, debug|release):DEFINES += QT_NO_DEBUG_OUTPUT

INCLUDEPATH += $$(MYLIBRARY)/include/analyzer
LIBS += -L$$(MYLIBRARY)/lib -l$$qtLibraryTarget(analyzer)

INCLUDEPATH += $$(MYLIBRARY)/include/Http
LIBS += -L$$(MYLIBRARY)/lib -l$$qtLibraryTarget(Http)

INCLUDEPATH += $$(MYLIBRARY)/include/QmlApplication
LIBS += -L$$(MYLIBRARY)/lib -l$$qtLibraryTarget(QmlApplication)

INCLUDEPATH += $$(MYLIBRARY)/include/Streaming
LIBS += -L$$(MYLIBRARY)/lib -l$$qtLibraryTarget(streaming)

INCLUDEPATH += $$(MYLIBRARY)/include/UpnpLibrary
LIBS += -L$$(MYLIBRARY)/lib -l$$qtLibraryTarget(UpnpLibrary)

INCLUDEPATH += $$(MYLIBRARY)/include/multimedia
LIBS += -L$$(MYLIBRARY)/lib -l$$qtLibraryTarget(mediadevice)

INCLUDEPATH += /usr/local/include
LIBS += -L/usr/local/lib -lavcodec -lavformat -lavutil -lswscale -lswresample

HEADERS += \
    myapplication.h \
    applicationcontroller.h \
    checknetworklinkitem.h \
    applicationworker.h \
    mediaimageprovider.h \
    albumimageprovider.h \
    formatimageprovider.h \
    mediaserver.h \
    mediamodel.h

SOURCES += \
    main.cpp \
    myapplication.cpp \
    applicationcontroller.cpp \
    checknetworklinkitem.cpp \
    applicationworker.cpp \
    mediaimageprovider.cpp \
    albumimageprovider.cpp \
    formatimageprovider.cpp \
    mediaserver.cpp \
    mediamodel.cpp

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
