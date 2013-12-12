QT += qml quick widgets network xml

TARGET = QT_Media_Server
TEMPLATE = app

SOURCES += \
    main.cpp

include (../common/common.pri)
include (../network/network.pri)

