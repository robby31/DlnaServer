INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS += \
    $$PWD/logger.h \
    $$PWD/application.h

SOURCES += \
    $$PWD/logger.cpp \
    $$PWD/application.cpp

OTHER_FILES += \
    $$PWD/PMS.xml \
    $$PWD/images/icon-256.png \
    $$PWD/images/icon-128.png \
    $$PWD/images/icon-128.jpg \
    $$PWD/images/icon-120.png \
    $$PWD/images/icon-120.jpg \
    $$PWD/images/icon-48.png \
    $$PWD/images/icon-48.jpg \
    $$PWD/images/icon-32.png \
    $$PWD/images/icon-22.png \
    $$PWD/images/icon-16.png \
    $$PWD/qml/main.qml \
    $$PWD/qml/content/RequestsView.qml

RESOURCES += \
    $$PWD/ressources.qrc
