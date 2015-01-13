QT += qml quick widgets network xml sql

TARGET = QT_Media_Server
TEMPLATE = app

SOURCES += \
    main.cpp

include (../common/common.pri)
include (../network/network.pri)

HEADERS += \
    application.h

SOURCES += \
    application.cpp

RESOURCES += \
    ressources.qrc


win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../QmlApplication/release/ -lQmlApplication
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../QmlApplication/debug/ -lQmlApplication
else:unix: LIBS += -L$$OUT_PWD/../../QmlApplication/ -lQmlApplication

INCLUDEPATH += $$PWD/../../QmlApplication
DEPENDPATH += $$PWD/../../QmlApplication
