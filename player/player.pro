QT += quick widgets sql webview multimedia xml
CONFIG += c++14

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

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp \
    myapplication.cpp \
    applicationworker.cpp \
    applicationcontroller.cpp \
    servermodel.cpp \
    serveritem.cpp \
    contentmodel.cpp \
    upnpclassimageprovider.cpp \
    mediarenderer.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    myapplication.h \
    applicationworker.h \
    applicationcontroller.h \
    servermodel.h \
    serveritem.h \
    contentmodel.h \
    upnpclassimageprovider.h \
    mediarenderer.h
