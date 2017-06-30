import QtQuick 2.5
import myTypes 1.0
import MyComponents 1.0
import "Pages"

MyApplication {
    id: mainWindow
    title: "QT Media Server"
    logoCompany: "qrc:///images/icon-32.png"
    minimumWidth: 800
    width: minimumWidth

    controller: homePageController
    modelButtons : mybuttons
    pages: myPages

    ListModel {
        id: mybuttons
        ListElement {
            title: "Request"
            state: "REQUESTS"
            icon: "qrc:/images/requests.png"
        }
        ListElement {
            title: "Renderers"
            state: "RENDERERS"
            icon: "qrc:/images/renderer.png"
        }
        ListElement {
            title: "Shared"
            state: "SHARED"
            icon: "qrc:/images/folder.png"
        }
        ListElement {
            title: "Navigator"
            state: "NAVIGATOR"
            icon: "qrc:/images/web.png"
        }
        ListElement {
            title: "Library"
            state: "LIBRARY"
            icon: "qrc:/images/media.png"
        }
        ListElement {
            title: "Debug"
            state: "DEBUG"
            icon: "qrc:/images/debug.png"
        }
    }

    Component {
        id: myPages

        ApplicationPages {

        }
    }
}
