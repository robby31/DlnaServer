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
        }
        ListElement {
            title: "Renderers"
            state: "RENDERERS"
        }
        ListElement {
            title: "Shared"
            state: "SHARED"
        }
        ListElement {
            title: "Navigator"
            state: "NAVIGATOR"
        }
    }

    Component {
        id: myPages

        ApplicationPages {

        }
    }
}
