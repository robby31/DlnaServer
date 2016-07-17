QT += qml quick widgets network xml sql webview

TARGET = QT_Media_Server
TEMPLATE = app

SOURCES += \
    main.cpp \
    myapplication.cpp \
    applicationcontroller.cpp \
    debugitem.cpp \
    createdatabasethread.cpp

include (../common/common.pri)
include (../network/network.pri)
include (../../QmlApplication/QmlApplication.prf)

HEADERS += \
    myapplication.h \
    applicationcontroller.h \
    debugitem.h \
    createdatabasethread.h

SOURCES +=

RESOURCES += \
    ressources.qrc


