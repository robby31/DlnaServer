QT += qml quick widgets network xml sql

TARGET = QT_Media_Server
TEMPLATE = app

SOURCES += \
    main.cpp

include (../common/common.pri)
include (../network/network.pri)
include (../../QmlApplication/QmlApplication.prf)

HEADERS += \
    application.h

SOURCES += \
    application.cpp

RESOURCES += \
    ressources.qrc


