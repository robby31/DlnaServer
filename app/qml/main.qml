import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.2
import myTypes 1.0
import MyComponents 1.0
import "Pages"

MyApplication {
    id: mainWindow
    title: "QT Media Server"
    minimumWidth: 600

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
    }

    Component {
        id: myPages

        ApplicationPages {

        }
    }
}
