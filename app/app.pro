QT += qml quick widgets network xml sql webview

TARGET = QT_Media_Server
TEMPLATE = app

SOURCES += \
    main.cpp \
    myapplication.cpp \
    applicationcontroller.cpp

include (../common/common.pri)
include (../network/network.pri)
include (../../QmlApplication/QmlApplication.prf)

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH += /Users/doudou/workspaceQT/qmlmodulesplugins

HEADERS += \
    myapplication.h \
    applicationcontroller.h

SOURCES +=

RESOURCES += \
    ressources.qrc


