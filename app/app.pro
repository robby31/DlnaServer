macx:QMAKE_MAC_SDK=macosx10.9

QT += qml quick widgets network xml sql

TARGET = QT_Media_Server
TEMPLATE = app

SOURCES += \
    main.cpp

include (../common/common.pri)
include (../network/network.pri)

